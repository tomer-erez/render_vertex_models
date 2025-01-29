#include "Edge.h"

// Constructor
Edge::Edge(Poly* p1, Poly* p2, std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2)
    : poly1(p1), poly2(p2), vertex1(v1), vertex2(v2) {}

// Getters
Poly* Edge::getPoly1() const {
    return poly1;
}

Poly* Edge::getPoly2() const {
    return poly2;
}

std::shared_ptr<Vertex> Edge::getVertex1() const {
    return vertex1;
}

std::shared_ptr<Vertex> Edge::getVertex2() const {
    return vertex2;
}

// Setters
void Edge::setPoly1(Poly* p1) {
    poly1 = p1;
}

void Edge::setPoly2(Poly* p2) {
    poly2 = p2;
}

// Check if the edge is a boundary edge
bool Edge::isBoundary() const {
    return poly2 == nullptr;
}

// Check if two vertices form the same edge (unordered)
bool Edge::isSameEdge(const std::shared_ptr<Vertex>& v1, const std::shared_ptr<Vertex>& v2) const {
    return (vertex1 == v1 && vertex2 == v2) || (vertex1 == v2 && vertex2 == v1);
}

// Comparison operator
bool Edge::operator==(const Edge& other) const {
    return isSameEdge(other.vertex1, other.vertex2) &&
        poly1 == other.poly1 && poly2 == other.poly2;
}
