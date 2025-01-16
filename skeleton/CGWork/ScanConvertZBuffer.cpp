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


// Helper struct for edge equations
struct EdgeEquation {
    float a, b, c;
    float yMin, yMax;

    EdgeEquation(const Vertex& v0, const Vertex& v1) {
        a = v0.y - v1.y;
        b = v1.x - v0.x;
        c = v0.x * v1.y - v1.x * v0.y;
        yMin = std::min(v0.y, v1.y);
        yMax = std::max(v0.y, v1.y);
    }

    bool findIntersection(float y, float& x) const {
        if (y < yMin || y > yMax) {
            return false;
        }
        if (std::abs(a) < 1e-6) {
            return false;
        }
        x = -(b * y + c) / a;
        return true;
    }
};

// Helper function for barycentric coordinate computation
inline float computeBarycentric(float x, float y,
    const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    return ((v0.y - v1.y) * (x - v1.x) + (v1.x - v0.x) * (y - v1.y)) /
        ((v0.y - v1.y) * (v2.x - v1.x) + (v1.x - v0.x) * (v2.y - v1.y));
}


// Render a polygon using scan-line rasterization and Z-buffering
int renderPolygon(Point* zBuffer, size_t width, size_t height, const Poly& polygon,
    const Vector4& cameraPosition, bool doBackFaceCulling) {
    const std::vector<Vertex>& vertices = polygon.getVertices();
    if (vertices.size() < 3) {
        return 0;
    }

    // Early bounds check for completely off-screen polygons
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& vertex : vertices) {
        minX = std::min(minX, vertex.x);
        maxX = std::max(maxX, vertex.x);
        minY = std::min(minY, vertex.y);
        maxY = std::max(maxY, vertex.y);
    }

    // Early exit if polygon is completely off screen
    if (maxX < 0 || minX >= width || maxY < 0 || minY >= height) {
        return 0;
    }

    // Back-face culling optimization
    if (doBackFaceCulling) {
        const Vector4 edge1 = vertices[1] - vertices[0];
        const Vector4 edge2 = vertices[2] - vertices[0];
        const Vector4 normal = edge1.cross(edge2).normalize();
        const Vector4 viewVector = (cameraPosition - vertices[0]).normalize();

        if (normal.dot(viewVector) <= 0) {
            return 0;
        }
    }

    // Clip bounding box to screen bounds
    minX = std::max(0.0f, std::floor(minX));
    maxX = std::min(static_cast<float>(width - 1), std::ceil(maxX));
    minY = std::max(0.0f, std::floor(minY));
    maxY = std::min(static_cast<float>(height - 1), std::ceil(maxY));

    const COLORREF color = polygon.getColor();

    // Pre-compute edge equations for the triangle
    std::vector<EdgeEquation> edges;
    edges.reserve(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vertex& v0 = vertices[i];
        const Vertex& v1 = vertices[(i + 1) % vertices.size()];
        edges.emplace_back(v0, v1);
    }

    // Scan-line rasterization with edge walking
    for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); ++y) {
        std::vector<float> xIntersections;
        xIntersections.reserve(edges.size());

        // Find intersections for all edges
        for (const auto& edge : edges) {
            float x;
            if (edge.findIntersection(static_cast<float>(y), x)) {
                xIntersections.push_back(x);
            }
        }

        if (xIntersections.empty()) {
            continue;
        }

        std::sort(xIntersections.begin(), xIntersections.end());

        // Process spans between intersection pairs
        for (size_t i = 0; i < xIntersections.size() - 1; i += 2) {
            const int startX = static_cast<int>(std::ceil(xIntersections[i]));
            const int endX = static_cast<int>(std::floor(xIntersections[i + 1]));

            // SIMD-friendly inner loop
            for (int x = startX; x <= endX; ++x) {
                const size_t index = static_cast<size_t>(y) * width + x;

                // Compute barycentric coordinates
                const float alpha = computeBarycentric(x, y, vertices[1], vertices[2], vertices[0]);
                const float beta = computeBarycentric(x, y, vertices[2], vertices[0], vertices[1]);
                const float gamma = 1.0f - alpha - beta;

                // Perspective-correct interpolation
                const float z = 1.0f / (alpha / vertices[0].z + beta / vertices[1].z + gamma / vertices[2].z);

                // Z-buffer test with atomic operation if possible
                if (z < zBuffer[index].z) {
                    zBuffer[index] = Point(static_cast<float>(x), static_cast<float>(y),
                        z, 1.0f, color, &polygon);
                }
            }
        }
    }

    return 1;
}

