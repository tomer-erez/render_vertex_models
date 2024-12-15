#include "Poly.h"
#include <iostream>

// Constructor
Poly::Poly()
    : hasNormal(false),
    color(RGB(255, 255, 255)),
    normalStart(Vector4()),
    normalEnd(Vector4()) {}

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
    normal = n.normalize(); // Ensure it's a unit vector
    hasNormal = true;
}

// Set the normal with visualization support
void Poly::setNormalWithVisualization(const Vector4& centroid, const Vector4& direction, double scale) {
    normalStart = centroid;
    normalEnd = centroid + direction.normalize() * scale;
    setNormal(direction); // Set the normalized direction
}

// Get the normal vector
const Vector4& Poly::getNormal() const {
    return normal;
}

// Get the start of the normal
const Vector4& Poly::getNormalStart() const {
    return normalStart;
}

// Get the end of the normal
const Vector4& Poly::getNormalEnd() const {
    return normalEnd;
}

// Check if the polygon has a normal
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
    setNormal(edge1.cross(edge2)); // Automatically normalizes and sets hasNormal = true
}

// Apply a transformation to the polygon
void Poly::applyTransform(const Matrix4& transform) {
    for (Vector4& vertex : vertices) {
        vertex = transform.transform(vertex);
    }
    for (Vector4& normal : vertexNormals) {
        normal = transform.transform(normal).normalize(); // Ensure normals remain unit vectors
    }
    if (hasNormal) {
        Matrix4 normalTransform = transform; // Exclude translation for normals
        normal = transform.transform(normal).normalize();
        normalStart = normalTransform.transform(normalStart);
        normalEnd = normalTransform.transform(normalEnd);
    }
}

// Calculate the bounding box
void Poly::calculateBoundingBox(Vector4& min, Vector4& max) const {
    min = Vector4(DBL_MAX, DBL_MAX, DBL_MAX, 1.0);
    max = Vector4(DBL_MIN, DBL_MIN, DBL_MIN, 1.0);

    for (const Vector4& vertex : vertices) {
        min.updateMin(vertex);
        max.updateMax(vertex);
    }
}
