#include "AntiAliasing.h"
#include <algorithm>  // For std::clamp
#include <vector>
#include <omp.h>      // OpenMP for parallelization
#include <string>
#include <cmath>      // For exp, sqrt, sin

double PI = 3.141592653589793;

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return max(minVal, min(maxVal, val));
}

void applyAntiAliasingByName(Point* buffer, int width, int height, int kernelSize, const std::string& filterName) {
    // Validate kernel size
    if (kernelSize != 3 && kernelSize != 5) {
        throw std::invalid_argument("Unsupported kernel size. Use 3 or 5.");
    }

    // Parallelize the nested loops using OpenMP for faster performance
#pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Point& point = buffer[y * width + x];

            if (point.z < 99999) { // Apply the filter only for valid depth values

                float sumR = 0, sumG = 0, sumB = 0, weightSum = 0;
                int halfSize = kernelSize / 2;
                for (int j = -halfSize; j <= halfSize; ++j) {
                    for (int i = -halfSize; i <= halfSize; ++i) {
                        // Calculate neighboring coordinates
                        int sampleX = clamp(x + i, 0, width - 1);
                        int sampleY = clamp(y + j, 0, height - 1);
                        Point& sample = buffer[sampleY * width + sampleX];

                        // Extract RGB components
                        COLORREF color = sample.getColor();
                        float r = static_cast<float>(GetRValue(color));
                        float g = static_cast<float>(GetGValue(color));
                        float b = static_cast<float>(GetBValue(color));

                        // Calculate weight based on the selected filter
                        float weight = 0.0f;

                        if (filterName == "Box") {
                            weight = 1.0f; // Uniform weight
                        }
                        else if (filterName == "Triangle") {
                            float distance = std::sqrt(i * i + j * j);
                            weight = max(0.0f, (halfSize - distance) / halfSize);
                        }
                        else if (filterName == "Gaussian") {
                            float sigma = 1.0f;
                            weight = std::exp(-(i * i + j * j) / (2 * sigma * sigma));
                        }
                        else if (filterName == "Sinc") {
                            float r = std::sqrt(i * i + j * j);
                            if (r <= halfSize) {
                                // Polynomial approximation of sinc
                                float sincValue;
                                if (r == 0) {
                                    sincValue = 1.0f;
                                }
                                else {
                                    float x = r;
                                    sincValue = 1.0f - (PI * PI * x * x) / 6.0f + (PI * PI * PI * PI * x * x * x * x) / 120.0f;
                                }

                                // Polynomial approximation of Hann window
                                float hannWindow = (PI * PI * r * r) / (4.0f * halfSize * halfSize)
                                    - (PI * PI * PI * PI * r * r * r * r) / (96.0f * halfSize * halfSize * halfSize * halfSize);

                                // Combined weight
                                weight = sincValue * hannWindow;
                                weight = clamp(weight, 0.0f, 255.0f);
                            }
                        }


                        // Accumulate weighted RGB values
                        sumR += r * weight;
                        sumG += g * weight;
                        sumB += b * weight;
                        weightSum += weight;
                    }
                }

                // Normalize weights to avoid division by zero
                if (weightSum == 0.0f) weightSum = 1.0f;

                // Compute final color
                
                point.setColor(RGB(
                    static_cast<int>(clamp(sumR / weightSum, 0.0f, 255.0f)),
                    static_cast<int>(clamp(sumG / weightSum, 0.0f, 255.0f)),
                    static_cast<int>(clamp(sumB / weightSum, 0.0f, 255.0f))
                ));
                
                //point.setColor(RGB(0, 0, 0));
            }
        }
    }
}
