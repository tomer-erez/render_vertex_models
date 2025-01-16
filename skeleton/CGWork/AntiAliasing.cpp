#include "AntiAliasing.h"
#include <algorithm>  // For std::clamp
#include <vector>
#include <omp.h>      // OpenMP for parallelization
#include <string>
#include <cmath>  // For exp, sqrt, sin
double PI = 3.1415;

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
}


void applyAntiAliasingByName(Point* buffer, int width, int height, int kernelSize, const std::string& filterName) {
    // Parallelize the nested loops using OpenMP for faster performance
    // 'collapse(2)' flattens the nested loops so that both `y` and `x` loops are parallelized.
#pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {  // Iterate over all rows (height)
        for (int x = 0; x < width; ++x) {  // Iterate over all columns (width)
            Point& point = buffer[y * width + x];  // Access the point (pixel) at (x, y)

            if (point.z < 99999) {  // Apply the filter only if the point's depth value is valid

                // Initialize sums for RGB values and weight sum
                float sumR = 0, sumG = 0, sumB = 0, weightSum = 0;
                int halfSize = kernelSize / 2;  // Calculate half the kernel size (e.g., 3x3 -> halfSize = 1)
                //halfSize = halfSize + 2;
                // Iterate over the kernel (a square around the point)
                for (int j = -halfSize; j <= halfSize; ++j) {  // Loop vertically within the kernel
                    for (int i = -halfSize; i <= halfSize; ++i) {  // Loop horizontally within the kernel
                        // Calculate the coordinates of the neighboring point
                        int sampleX = clamp(x + i, 0, width - 1);  // Clamp to prevent out-of-bounds access
                        int sampleY = clamp(y + j, 0, height - 1);  // Clamp to prevent out-of-bounds access
                        Point& sample = buffer[sampleY * width + sampleX];  // Get the neighboring point

                        // Get the color of the neighboring point
                        COLORREF color = sample.getColor();
                        float r = static_cast<float>(GetRValue(color));  // Extract the red component
                        float g = static_cast<float>(GetGValue(color));  // Extract the green component
                        float b = static_cast<float>(GetBValue(color));  // Extract the blue component

                        float weight = 0.0f;  // Weight for the current point based on the filter

                        // Apply appropriate weight based on the selected filter type
                        if (filterName == "Box") {
                            if (i == 0 && j == 0) {
                                weight = 0;
                            }
                            // Box filter: uniform weight of 1 for all neighboring points
                            weight = 1.0f;
                        }
                        else if (filterName == "Triangle") {
                            // Triangle filter: weights decrease linearly with distance from the center
                            float distance = std::sqrt(i * i + j * j);  // Euclidean distance from the center
                            weight = max(0.0f, (halfSize - distance) / halfSize);  // Linear falloff
                        }
                        else if (filterName == "Gaussian") {
                            // Gaussian filter: weights follow a Gaussian curve (bell-shaped)
                            float sigma = 1.0f;  // Standard deviation for the Gaussian distribution
                            weight = exp(-(i * i + j * j) / (2 * sigma * sigma));  // Exponential decay based on distance
                        }
                        else if (filterName == "Sinc") {
                            float r = std::sqrt(i * i + j * j);  // Distance from the center
                            if (r <= halfSize) {
                                // Sinc function with Hann window
                                float sincValue = (r == 0) ? 1.0f : sin(3.14159265f * r) / (3.14159265f * r);  // Sinc function
                                float hannWindow = 0.5f * (1 - cos(3.14159265f * r / halfSize));  // Hann window
                                weight = sincValue * hannWindow;  // Combine sinc and Hann window
                            }
                            else {
                                weight = 0.0f;  // Outside kernel range
                            }
                        }

                        // Accumulate the weighted contributions for the red, green, and blue channels
                        sumR += r * weight;
                        sumG += g * weight;
                        sumB += b * weight;
                        weightSum += weight;  // Accumulate the total weight
                    }
                }

                // Avoid division by zero in case the weight sum is zero
                if (weightSum == 0.0f) {
                    weightSum = 1.0f;
                }

                // Compute the final color by dividing the weighted sum of RGB values by the total weight
                point.setColor(RGB(
                    static_cast<int>(sumR / weightSum),  // Final red value
                    static_cast<int>(sumG / weightSum),  // Final green value
                    static_cast<int>(sumB / weightSum)   // Final blue value
                ));
            }
        }
    }
}
