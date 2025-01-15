#include "AntiAliasing.h"
#include <cmath>
#include <algorithm>  // For std::clamp
#include <vector>
#include <omp.h>      // OpenMP for parallelization
template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
}

float sinc(float x) {
    if (x == 0) return 1.0f;
    return sin(3.14159265f * x) / (3.14159265f * x);
}

template <typename FilterFunc>
Point applyKernel(Point* buffer, int width, int height, int x, int y, int kernelSize, FilterFunc filter) {
    int halfSize = kernelSize / 2;
    float sumR = 0, sumG = 0, sumB = 0, weightSum = 0;

    // Precompute weights for performance
    std::vector<std::vector<float>> weights(kernelSize, std::vector<float>(kernelSize));

    for (int j = -halfSize; j <= halfSize; ++j) {
        for (int i = -halfSize; i <= halfSize; ++i) {
            weights[j + halfSize][i + halfSize] = filter(i, j);
        }
    }

    for (int j = -halfSize; j <= halfSize; ++j) {
        for (int i = -halfSize; i <= halfSize; ++i) {
            int sampleX =clamp(x + i, 0, width - 1);
            int sampleY = clamp(y + j, 0, height - 1);
            Point& sample = buffer[sampleY * width + sampleX];

            // Extract RGB values using getColor()
            COLORREF color = sample.getColor();
            float r = static_cast<float>(GetRValue(color));
            float g = static_cast<float>(GetGValue(color));
            float b = static_cast<float>(GetBValue(color));

            float weight = weights[j + halfSize][i + halfSize];
            sumR += r * weight;
            sumG += g * weight;
            sumB += b * weight;
            weightSum += weight;
        }
    }

    // Avoid division by zero
    if (weightSum == 0.0f) weightSum = 1.0f;

    Point result;
    result.setColor(RGB(
        static_cast<int>(sumR / weightSum),
        static_cast<int>(sumG / weightSum),
        static_cast<int>(sumB / weightSum)
    ));
    return result;
}

void applyBoxFilter(Point* buffer, int width, int height, int x, int y, int kernelSize) {
    auto boxFilter = [](int, int) { return 1.0f; };  // Equal weight for box filter
    buffer[y * width + x] = applyKernel(buffer, width, height, x, y, kernelSize, boxFilter);
}

void applyTriangleFilter(Point* buffer, int width, int height, int x, int y, int kernelSize) {
    auto triangleFilter = [kernelSize](int i, int j) {
        int halfSize = kernelSize / 2;
        float distance = std::sqrt(i * i + j * j);
        return max(0.0f, (halfSize - distance) / halfSize);
    };
    buffer[y * width + x] = applyKernel(buffer, width, height, x, y, kernelSize, triangleFilter);
}

void applyGaussianFilter(Point* buffer, int width, int height, int x, int y, int kernelSize) {
    auto gaussianFilter = [](int i, int j) {
        float sigma = 1.0f;
        return exp(-(i * i + j * j) / (2 * sigma * sigma));
    };
    buffer[y * width + x] = applyKernel(buffer, width, height, x, y, kernelSize, gaussianFilter);
}

void applySincFilter(Point* buffer, int width, int height, int x, int y, int kernelSize) {
    auto sincFilter = [](int i, int j) {
        float r = std::sqrt(i * i + j * j);
        return sinc(r);
    };
    buffer[y * width + x] = applyKernel(buffer, width, height, x, y, kernelSize, sincFilter);
}

void applyAntiAliasing(Point* buffer, int width, int height, int kernelSize, void (*filterFunc)(Point*, int, int, int, int, int)) {
    // Apply anti-aliasing in parallel for performance
#pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            filterFunc(buffer, width, height, x, y, kernelSize);
        }
    }
}
