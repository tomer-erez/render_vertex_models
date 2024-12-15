#include "Poly.h"
#include <iostream>

// Constructor
Poly::Poly() : hasNormal(false), color(RGB(255, 255, 255)) {} // Default color is white

// Add a vertex to the polygon
void Poly::addVertex(const Vector4& vertex) {
    vertices.push_back(vertex);
}

// Add a vertex normal
void Poly::addVertexNormal(const Vector4& normal) {
    vertexNormals.push_back(normal);
}

// Get vertices
std::vector<Vector4>& Poly::getVertices() {
    return vertices;
}

const std::vector<Vector4>& Poly::getVertices() const {
    return vertices;
}

// Get vertex normals
std::vector<Vector4>& Poly::getVertexNormals() {
    return vertexNormals;
}

const std::vector<Vector4>& Poly::getVertexNormals() const {
    return vertexNormals;
}

// Get the number of vertices
size_t Poly::getVertexCount() const {
    return vertices.size();
}

// Set polygon color
void Poly::setColor(COLORREF c) {
    color = c;
}

// Get polygon color
COLORREF Poly::getColor() const {
    return color;
}

// Set the normal vector
void Poly::setNormal(const Vector4& n) {
    normal = n;
    hasNormal = true;
}

// Get the normal vector
const Vector4& Poly::getNormal() const {
    return normal;
}

// Check if the polygon has a predefined normal
bool Poly::hasPredefinedNormal() const {
    return hasNormal;
}

// Calculate the normal from vertices
void Poly::calculateNormal() {
    if (vertices.size() < 3) {
        std::cerr << "Cannot calculate normal: Less than 3 vertices in the polygon." << std::endl;
        return;
    }

    Vector4 edge1 = vertices[1] - vertices[0];
    Vector4 edge2 = vertices[2] - vertices[0];
    normal = edge1.cross(edge2).normalize();
    hasNormal = true;
}

// Apply a transformation to the polygon
void Poly::applyTransform(const Matrix4& transform) {
    for (Vector4& vertex : vertices) {
        vertex = transform.transform(vertex);
    }
    for (Vector4& normal : vertexNormals) {
        normal = transform.transform(normal).normalize();
    }
    if (hasNormal) {
        normal = transform.transform(normal).normalize();
    }
}

// Calculate the bounding box
void Poly::calculateBoundingBox(Vector4& min, Vector4& max) const {
    min = Vector4(DBL_MAX, DBL_MAX, DBL_MAX,DBL_MAX);
    max = Vector4(DBL_MIN, DBL_MIN, DBL_MIN, DBL_MIN);

    for (const Vector4& vertex : vertices) {
        min.updateMin(vertex);
        max.updateMax(vertex);
    }
}

// Debugging: Print polygon details
void Poly::print() const {
    std::cout << "Polygon with " << getVertexCount() << " vertices:" << std::endl;
    for (const auto& vertex : vertices) {
    }
    if (hasNormal) {
        std::cout << "Normal: ";
    }
    std::cout << "Color: ("
        << GetRValue(color) << ", "
        << GetGValue(color) << ", "
        << GetBValue(color) << ")" << std::endl;
}
