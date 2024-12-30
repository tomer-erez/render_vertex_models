#define NOMINMAX // Prevent min and max macros from being defined
#include "ScanConvertZBuffer.h"
#include "Point.h"
#include <algorithm> // For std::min, std::max
#include <cstdlib>   // For malloc and free
#include <cfloat>    // For FLT_MAX

// Initialize the Z-buffer
Point* initZBuffer(size_t width, size_t height) {
    Point* zBuffer = (Point*)malloc(width * height * sizeof(Point));

    for (size_t i = 0; i < width * height; i++) {
        zBuffer[i] = Point(0.0f, 0.0f, FLT_MAX, 1.0f, RGB(0, 0, 0)); // Max depth and black color
    }

    return zBuffer;
}

// Free the Z-buffer
void freeZBuffer(Point* zBuffer) {
    free(zBuffer);
}

// Interpolate color between two colors
COLORREF interpolateColor(COLORREF c1, COLORREF c2, float t) {
    uint8_t r1 = GetRValue(c1), g1 = GetGValue(c1), b1 = GetBValue(c1);
    uint8_t r2 = GetRValue(c2), g2 = GetGValue(c2), b2 = GetBValue(c2);

    uint8_t r = static_cast<uint8_t>(r1 + t * (r2 - r1));
    uint8_t g = static_cast<uint8_t>(g1 + t * (g2 - g1));
    uint8_t b = static_cast<uint8_t>(b1 + t * (b2 - b1));

    return RGB(r, g, b);
}

// Render a polygon using scan conversion and Z-buffering
void renderPolygon(Point* zBuffer, size_t width, size_t height, const std::vector<Point>& polygon) {
    if (polygon.size() < 3) return; // Polygons must have at least 3 vertices

    // Find the bounding box of the polygon
    float minX = std::min_element(polygon.begin(), polygon.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
        })->x;
    float maxX = std::max_element(polygon.begin(), polygon.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
        })->x;

    float minY = std::min_element(polygon.begin(), polygon.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
        })->y;
    float maxY = std::max_element(polygon.begin(), polygon.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
        })->y;

    // Clip to screen bounds
    minX = std::max(minX, 0.0f);
    maxX = std::min(maxX, static_cast<float>(width - 1));
    minY = std::max(minY, 0.0f);
    maxY = std::min(maxY, static_cast<float>(height - 1));

    // Loop through each pixel in the bounding box
    for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); y++) {
        for (int x = static_cast<int>(minX); x <= static_cast<int>(maxX); x++) {
            float wSum = 0.0f;
            bool inside = true;
            std::vector<float> weights(polygon.size(), 0.0f);

            // Compute weights for each vertex
            for (size_t i = 0; i < polygon.size(); i++) {
                const Point& p1 = polygon[i];
                const Point& p2 = polygon[(i + 1) % polygon.size()];

                float edgeW = (p2.x - p1.x) * (y - p1.y) - (p2.y - p1.y) * (x - p1.x);
                if (edgeW < 0) {
                    inside = false;
                    break;
                }
                weights[i] = edgeW;
                wSum += edgeW;
            }

            if (!inside || wSum == 0.0f) continue;

            // Normalize weights
            for (float& weight : weights) {
                weight /= wSum;
            }

            // Interpolate Z and color using weights
            float z = 0.0f;
            COLORREF color = RGB(0, 0, 0);
            for (size_t i = 0; i < polygon.size(); i++) {
                z += weights[i] * polygon[i].z;
                color = interpolateColor(color, polygon[i].getColor(), weights[i]);
            }

            // Z-buffer test
            size_t index = y * width + x;
            if (z < zBuffer[index].z) {
                zBuffer[index] = Point(static_cast<float>(x), static_cast<float>(y), z, 1.0f, color);
            }
        }
    }
}
