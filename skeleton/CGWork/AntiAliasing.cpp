#include <omp.h>
#include <vector>
#include <cmath>
#include <windows.h>
#include "AntiAliasing.h"
#include <algorithm>
#include <vector>
#include <omp.h>
#include <string>
#include "Point.h"
#include <cmath>      // For exp, sqrt, sin
#include <cstdlib>    // For malloc and free
#include <windows.h>  // For GetRValue, GetGValue, GetBValue
enum ColorChannel { RED, GREEN, BLUE };
float M_PI = 3.14159;
// Clamp function
template <typename T>
inline T clamp(T val, T minVal, T maxVal) {
    return max(minVal, min(maxVal, val));
}

// Linear interpolation
inline float lerpit(float a, float b, float t) {
    return a + t * (b - a);
}

// Interpolate channel
inline float interpolateChannel(const Point& p00, const Point& p10, const Point& p01, const Point& p11,
    float wx, float wy, ColorChannel channel) {
    float i1, i2;
    if (channel == RED) {
        i1 = lerpit(GetRValue(p00.getColor()), GetRValue(p10.getColor()), wx);
        i2 = lerpit(GetRValue(p01.getColor()), GetRValue(p11.getColor()), wx);
    }
    else if (channel == GREEN) {
        i1 = lerpit(GetGValue(p00.getColor()), GetGValue(p10.getColor()), wx);
        i2 = lerpit(GetGValue(p01.getColor()), GetGValue(p11.getColor()), wx);
    }
    else {  // BLUE
        i1 = lerpit(GetBValue(p00.getColor()), GetBValue(p10.getColor()), wx);
        i2 = lerpit(GetBValue(p01.getColor()), GetBValue(p11.getColor()), wx);
    }
    return lerpit(i1, i2, wy);
}

// Precompute weights for a given filter
std::vector<float> precomputeWeights(int ssaaFactor, const std::string& filterName) {
    const float step = 1.0f / ssaaFactor;
    std::vector<float> weights(ssaaFactor * ssaaFactor);
    float weightSum = 0.0f;

    if (filterName == "Gaussian") {
        const float sigma = 0.5f;
        for (int sy = 0; sy < ssaaFactor; ++sy) {
            for (int sx = 0; sx < ssaaFactor; ++sx) {
                float dx = (sx + 0.5f) * step - 0.5f;
                float dy = (sy + 0.5f) * step - 0.5f;
                float weight = std::exp(-(dx * dx + dy * dy) / (2.0f * sigma * sigma));
                weights[sy * ssaaFactor + sx] = weight;
                weightSum += weight;
            }
        }
    }
    else if (filterName == "Triangle") {
        for (int sy = 0; sy < ssaaFactor; ++sy) {
            for (int sx = 0; sx < ssaaFactor; ++sx) {
                float dx = std::fabs((sx + 0.5f) * step - 0.5f);
                float dy = std::fabs((sy + 0.5f) * step - 0.5f);
                float weight = max(0.0f, 1.0f - (dx + dy));
                weights[sy * ssaaFactor + sx] = weight;
                weightSum += weight;
            }
        }
    }
    else if (filterName == "Sinc") {
        const float sincScale = 5.50f; // Controls the width of the sinc function
        for (int sy = 0; sy < ssaaFactor; ++sy) {
            for (int sx = 0; sx < ssaaFactor; ++sx) {
                float dx = (sx + 0.5f) * step - 0.5f;
                float dy = (sy + 0.5f) * step - 0.5f;

                // Compute 2D sinc weight
                float r = std::sqrt(dx * dx + dy * dy) * sincScale;
                float sincWeight = (r == 0.0f) ? 1.0f : std::sin(M_PI * r) / (M_PI * r);

                weights[sy * ssaaFactor + sx] = sincWeight;
                weightSum += sincWeight;
            }
        }
    }
    else {
        for (int i = 0; i < ssaaFactor * ssaaFactor; ++i) {
            weights[i] = 1.0f;  // Box filter
            weightSum += 1.0f;
        }
    }

    for (float& weight : weights) {
        weight /= weightSum;
    }
    return weights;
}

// Apply anti-aliasing
void applyAntiAliasingByName(Point* buffer, int width, int height, int kernelSize, const std::string& filterName) {
    const int ssaaFactor = kernelSize+6;
    const float step = 1.0f / ssaaFactor;
    const auto weights = precomputeWeights(ssaaFactor, filterName);

    std::vector<Point> resultBuffer(width * height);

#pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

            for (int sy = 0; sy < ssaaFactor; ++sy) {
                for (int sx = 0; sx < ssaaFactor; ++sx) {
                    float subX = x + (sx + 0.5f) * step;
                    float subY = y + (sy + 0.5f) * step;

                    int x0 = clamp(static_cast<int>(std::floor(subX)), 0, width - 1);
                    int y0 = clamp(static_cast<int>(std::floor(subY)), 0, height - 1);
                    int x1 = clamp(x0 + 1, 0, width - 1);
                    int y1 = clamp(y0 + 1, 0, height - 1);

                    const Point& p00 = buffer[y0 * width + x0];
                    const Point& p10 = buffer[y0 * width + x1];
                    const Point& p01 = buffer[y1 * width + x0];
                    const Point& p11 = buffer[y1 * width + x1];

                    float wx = subX - x0;
                    float wy = subY - y0;

                    float weight = weights[sy * ssaaFactor + sx];

                    sumR += interpolateChannel(p00, p10, p01, p11, wx, wy, RED) * weight;
                    sumG += interpolateChannel(p00, p10, p01, p11, wx, wy, GREEN) * weight;
                    sumB += interpolateChannel(p00, p10, p01, p11, wx, wy, BLUE) * weight;
                }
            }

            resultBuffer[y * width + x].setColor(RGB(
                static_cast<int>(clamp(sumR, 0.0f, 255.0f)),
                static_cast<int>(clamp(sumG, 0.0f, 255.0f)),
                static_cast<int>(clamp(sumB, 0.0f, 255.0f))
            ));
        }
    }

#pragma omp parallel for
    for (int i = 0; i < width * height; ++i) {
        buffer[i] = resultBuffer[i];
    }
}
