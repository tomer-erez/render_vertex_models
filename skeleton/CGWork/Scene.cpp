#include "Scene.h"
#include <algorithm>
#include <limits>

// Constructor
Scene::Scene()
    : sceneTransform(Matrix4()),
    boundingBox{ Vector4(DBL_MAX, DBL_MAX, DBL_MAX,DBL_MAX), Vector4(DBL_MIN, DBL_MIN, DBL_MIN,DBL_MIN) },
    wireframeColor(RGB(255, 255, 255)),  // Default white
    normalColor(RGB(0, 255, 0)),        // Default green
    backgroundColor(RGB(0, 0, 0)),      // Default black
    sensitivity(1.0),                   // Default sensitivity
    showNormals(false),                 // Default: don't show normals
    showBoundingBox(false) {}           // Default: don't show bounding box

// Add a polygon to the scene
void Scene::addPolygon(const Poly& poly) {
    polygons.push_back(poly);
    calculateBoundingBox(); // Recalculate bounding box
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
        for (Vector4& vertex : poly.getVertices()) { // Use mutable getVertices()
            vertex = transform.transform(vertex); // Transform each vertex
        }
    }
    calculateBoundingBox(); // Update the bounding box after transformation
}

// Calculate the bounding box of the scene
void Scene::calculateBoundingBox() {
    boundingBox.min = Vector4(DBL_MAX,DBL_MAX, DBL_MAX, DBL_MAX);
    boundingBox.max = Vector4(DBL_MIN,DBL_MIN, DBL_MIN, DBL_MIN);

    for (const Poly& poly : polygons) {
        for (const Vector4& vertex : poly.getVertices()) {
            boundingBox.min.updateMin(vertex);
            boundingBox.max.updateMax(vertex);
        }
    }
}

// Get the bounding box of the scene
const BoundingBox& Scene::getBoundingBox() const {
    return boundingBox;
}

// Set custom colors for wireframe, normals, and background
void Scene::setColors(COLORREF wireframe, COLORREF normal, COLORREF background) {
    wireframeColor = wireframe;
    normalColor = normal;
    backgroundColor = background;
}

// Get custom colors
COLORREF Scene::getWireframeColor() const {
    return wireframeColor;
}

COLORREF Scene::getNormalColor() const {
    return normalColor;
}

COLORREF Scene::getBackgroundColor() const {
    return backgroundColor;
}

// Set sensitivity for transformations
void Scene::setSensitivity(double newSensitivity) {
    sensitivity = newSensitivity;
}

// Get sensitivity value
double Scene::getSensitivity() const {
    return sensitivity;
}

// Flags for rendering options
void Scene::setShowNormals(bool show) {
    showNormals = show;
}

bool Scene::isShowNormals() const {
    return showNormals;
}

void Scene::setShowBoundingBox(bool show) {
    showBoundingBox = show;
}

bool Scene::isShowBoundingBox() const {
    return showBoundingBox;
}

// Clear the scene
void Scene::clear() {
    polygons.clear();
    sceneTransform = Matrix4(); // Reset to identity matrix
    boundingBox = { Vector4(DBL_MAX, DBL_MAX, DBL_MAX), Vector4(DBL_MIN, DBL_MIN, DBL_MIN) };
}
