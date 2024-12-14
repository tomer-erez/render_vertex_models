#ifndef POLY_H
#define POLY_H

#include <vector>
#include "Vector3.h"

class Poly {
private:
    std::vector<Vector3> vertices; // List of vertices that make up the polygon

public:
    // Add a vertex to the polygon
    void addVertex(const Vector3& vertex);

    // Mutable version: Get the vertices for modification
    std::vector<Vector3>& getVertices() { return vertices; }

    // Const version: Get the vertices for read-only access
    const std::vector<Vector3>& getVertices() const { return vertices; }

    // Get the number of vertices
    size_t getVertexCount() const;
};

#endif // POLY_H
