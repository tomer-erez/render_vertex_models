#ifndef POLY_H
#define POLY_H

#include <vector>
#include "Vector3.h"
#include "Matrix4.h"
#include <Windows.h> // For COLORREF

class Poly {
private:
    std::vector<Vector3> vertices;       // List of vertices
    std::vector<Vector3> vertexNormals;  // Normals for each vertex
    Vector3 normal;                      // Normal vector for the polygon
    bool hasNormal;                      // Indicates if the polygon has a predefined normal
    COLORREF color;                      // Color of the polygon

public:
    // Constructor
    Poly();

    // Add a vertex and its normal (optional)
    void addVertex(const Vector3& vertex);
    void addVertexNormal(const Vector3& normal);

    // Get vertices
    std::vector<Vector3>& getVertices();
    const std::vector<Vector3>& getVertices() const;

    // Get vertex normals
    std::vector<Vector3>& getVertexNormals();
    const std::vector<Vector3>& getVertexNormals() const;

    // Get the number of vertices
    size_t getVertexCount() const;

    // Polygon color management
    void setColor(COLORREF c);
    COLORREF getColor() const;

    // Polygon normal management
    void setNormal(const Vector3& normal);
    const Vector3& getNormal() const;
    bool hasPredefinedNormal() const;

    // Calculate normal from vertices
    void calculateNormal();

    // Apply a transformation to the polygon
    void applyTransform(const Matrix4& transform);

    // Calculate the bounding box (optional)
    void calculateBoundingBox(Vector3& min, Vector3& max) const;

    // Debugging: Print polygon details
    void print() const;
};

#endif // POLY_H
