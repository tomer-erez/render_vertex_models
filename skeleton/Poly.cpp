#include "Poly.h"

// Add a vertex to the polygon
void Poly::addVertex(const Vector3& vertex) {
    vertices.push_back(vertex);
}



// Get the number of vertices
size_t Poly::getVertexCount() const {
    return vertices.size();
}
