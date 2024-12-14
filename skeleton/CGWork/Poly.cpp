#include "Poly.h"
#include <iostream>

// Constructor
Poly::Poly() : hasNormal(false) {}

// Add a vertex to the polygon
void Poly::addVertex(const Vector3& vertex) {
    vertices.push_back(vertex);
}

// Mutable version: Get the vertices for modification
std::vector<Vector3>& Poly::getVertices() {
    return vertices;
}

// Const version: Get the vertices for read-only access
const std::vector<Vector3>& Poly::getVertices() const {
    return vertices;
}

// Get the number of vertices
size_t Poly::getVertexCount() const {
    return vertices.size();
}

// Set the normal vector
void Poly::setNormal(const Vector3& n) {
    normal = n;
    hasNormal = true;
}

// Get the normal vector
const Vector3& Poly::getNormal() const {
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

    // Use the first three vertices to compute the normal
    Vector3 edge1 = vertices[1] - vertices[0];
    Vector3 edge2 = vertices[2] - vertices[0];
    normal = edge1.cross(edge2).normalize();
    hasNormal = true;
}

// Apply a transformation to the polygon
void Poly::applyTransform(const Matrix4& transform) {
    for (Vector3& vertex : vertices) {
        vertex = transform.transform(vertex);
    }
    if (hasNormal) {
        normal = transform.transform(normal).normalize();
    }
}

// Debugging: Print polygon details
void Poly::print() const {
    std::cout << "Polygon with " << getVertexCount() << " vertices:" << std::endl;
    for (const auto& vertex : vertices) {
        vertex.print();
    }
    if (hasNormal) {
        std::cout << "Normal: ";
        normal.print();
    }
}
