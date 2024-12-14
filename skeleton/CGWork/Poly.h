#ifndef POLY_H
#define POLY_H

#include <vector>
#include "Vector3.h"
#include "Matrix4.h"

class Poly {
private:
    std::vector<Vector3> vertices;   // List of vertices
    Vector3 normal;                  // Normal vector for the polygon
    bool hasNormal;                  // Indicates if the polygon has a predefined normal

public:
    // Constructor
    Poly();

    // Add a vertex to the polygon
    void addVertex(const Vector3& vertex);

    // Mutable version: Get the vertices for modification
    std::vector<Vector3>& getVertices();

    // Const version: Get the vertices for read-only access
    const std::vector<Vector3>& getVertices() const;

    // Get the number of vertices
    size_t getVertexCount() const;

    // Set and get the normal vector
    void setNormal(const Vector3& normal);
    const Vector3& getNormal() const;
    bool hasPredefinedNormal() const;

    // Calculate normal from vertices (if not predefined)
    void calculateNormal();

    // Apply a transformation to the polygon
    void applyTransform(const Matrix4& transform);

    // Debugging: Print polygon details
    void print() const;
};

#endif // POLY_H
