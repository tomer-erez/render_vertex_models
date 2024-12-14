#include "Scene.h"

Scene::Scene() : sceneTransform(Matrix4()) {} // Initialize with an identity matrix

// Add a polygon to the scene
void Scene::addPolygon(const Poly& poly) {
    polygons.push_back(poly);
}

// Get all polygons in the scene
const std::vector<Poly>& Scene::getPolygons() const {
    return polygons;
}

// Get the total number of polygons
size_t Scene::getPolygonCount() const {
    return polygons.size();
}

// Apply a transformation to the entire scene
void Scene::applyTransform(const Matrix4& transform) {
    sceneTransform = sceneTransform * transform; // Accumulate transformations
    for (Poly& poly : polygons) {
        for (Vector3& vertex : poly.getVertices()) { // Use mutable getVertices()
            vertex = transform.transform(vertex); // Transform each vertex
        }
    }
}

// Clear the scene (reset the polygons and transformation)
void Scene::clear() {
    polygons.clear();
    sceneTransform = Matrix4(); // Reset to identity matrix
}
