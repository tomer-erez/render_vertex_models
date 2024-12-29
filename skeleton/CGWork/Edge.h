#ifndef EDGE_H
#define EDGE_H

#include "Poly.h"
#include "Vertex.h"
#include <memory>
#include <utility>

class Edge {
private:
    Poly* poly1;        // First polygon
    Poly* poly2;        // Second polygon (nullptr if boundary edge)
    std::shared_ptr<Vertex> vertex1; // First vertex of the edge
    std::shared_ptr<Vertex> vertex2; // Second vertex of the edge

public:
    // Constructor
    Edge(Poly* p1, Poly* p2, std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);

    // Getters
    Poly* getPoly1() const;
    Poly* getPoly2() const;
    std::shared_ptr<Vertex> getVertex1() const;
    std::shared_ptr<Vertex> getVertex2() const;

    // Setters
    void setPoly1(Poly* p1);
    void setPoly2(Poly* p2);

    // Helper to check if the edge is a boundary edge
    bool isBoundary() const;

    // Helper to check equality (unordered)
    bool isSameEdge(const std::shared_ptr<Vertex>& v1, const std::shared_ptr<Vertex>& v2) const;

    // Comparison operator for consistency in unordered structures
    bool operator==(const Edge& other) const;

    // Hash function for unordered_map or unordered_set
    struct Hash {
        size_t operator()(const std::pair<std::shared_ptr<Vertex>, std::shared_ptr<Vertex>>& edge) const {
            auto hashPtr = [](const std::shared_ptr<Vertex>& ptr) -> size_t {
                return std::hash<void*>()(ptr.get());
            };
            return hashPtr(edge.first) ^ (hashPtr(edge.second) << 1);
        }
    };
};

#endif // EDGE_H