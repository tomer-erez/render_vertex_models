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
    return RGB(
        static_cast<uint8_t>(GetRValue(c1) + t * (GetRValue(c2) - GetRValue(c1))),
        static_cast<uint8_t>(GetGValue(c1) + t * (GetGValue(c2) - GetGValue(c1))),
        static_cast<uint8_t>(GetBValue(c1) + t * (GetBValue(c2) - GetBValue(c1)))
    );
}

// Render a polygon using scan-line rasterization and Z-buffering
void renderPolygon(Point* zBuffer, size_t width, size_t height, const Poly& polygon, const Vector4& cameraPosition, bool doBackFaceCulling) {
    const std::vector<Vertex>& vertices = polygon.getVertices();
    if (vertices.size() < 3) {
        return; // Polygons must have at least 3 vertices
    }

    // Compute the polygon's normal
    const Vertex& v0 = vertices[0];
    const Vertex& v1 = vertices[1];
    const Vertex& v2 = vertices[2];

    if (doBackFaceCulling) {
        Vector4 edge1 = v1 - v0;
        Vector4 edge2 = v2 - v0;
        Vector4 normal = edge1.cross(edge2).normalize();
        Vector4 viewVector = (cameraPosition - v0).normalize();

        // Perform back-face culling
        if (normal.dot(viewVector) <= 0) {
            return; // Skip rendering back-facing polygons
        }
    }

    // Polygon's color
    COLORREF color = polygon.getColor();

    // Calculate bounding box
    float minX = std::floor(std::min({ v0.x, v1.x, v2.x }));
    float maxX = std::ceil(std::max({ v0.x, v1.x, v2.x }));
    float minY = std::floor(std::min({ v0.y, v1.y, v2.y }));
    float maxY = std::ceil(std::max({ v0.y, v1.y, v2.y }));

    // Clip bounding box to screen bounds
    minX = std::max(0.0f, minX);
    maxX = std::min(static_cast<float>(width - 1), maxX);
    minY = std::max(0.0f, minY);
    maxY = std::min(static_cast<float>(height - 1), maxY);

    // Compute edge equations for barycentric interpolation
    Vector4 edge1 = v1 - v0;
    Vector4 edge2 = v2 - v0;
    float denom = edge1.x * edge2.y - edge1.y * edge2.x;

    // Loop through scan lines
    for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); y++) {
        std::vector<float> xIntersections;

        // Find X-intersections with polygon edges
        for (size_t i = 0; i < vertices.size(); i++) {
            const Vertex& vStart = vertices[i];
            const Vertex& vEnd = vertices[(i + 1) % vertices.size()];

            if (vStart.y == vEnd.y) continue; // Skip horizontal edges

            if (y >= std::min(vStart.y, vEnd.y) && y <= std::max(vStart.y, vEnd.y)) {
                float x = vStart.x + (y - vStart.y) * (vEnd.x - vStart.x) / (vEnd.y - vStart.y);
                xIntersections.push_back(x);
            }
        }

        // Sort X-intersections to determine spans
        std::sort(xIntersections.begin(), xIntersections.end());

        // Fill spans between pairs of X-intersections
        for (size_t i = 0; i < xIntersections.size(); i += 2) {
            if (i + 1 >= xIntersections.size()) break;

            int startX = static_cast<int>(std::ceil(xIntersections[i]));
            int endX = static_cast<int>(std::floor(xIntersections[i + 1]));

            for (int x = startX; x <= endX; x++) {
                if (x < 0 || x >= static_cast<int>(width)) continue;

                // Interpolate Z-value using barycentric coordinates
                float alpha = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / denom;
                float beta = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / denom;
                float gamma = 1.0f - alpha - beta;

                float z = alpha * v0.z + beta * v1.z + gamma * v2.z;

                // Perform Z-buffer test
                size_t index = y * width + x;
                if (z < zBuffer[index].z) {
                    zBuffer[index] = Point(static_cast<float>(x), static_cast<float>(y), z, 1.0f, color,&polygon);
                }
            }
        }
    }
}
