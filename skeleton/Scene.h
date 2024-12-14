#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Poly.h"
#include "Matrix4.h"
#include "Light.h" // Optional: For handling lights in the scene

class Scene {
private:
    std::vector<Poly> polygons;   // List of polygons in the scene
    Matrix4 sceneTransform;       // Transformation matrix for the entire scene

    // Optional: Add camera or other scene properties here
    // Camera camera;             // Camera parameters
    // RGBColor backgroundColor;  // Background color of the scene

public:
    // Constructor
    Scene();

    // Add a polygon to the scene
    void addPolygon(const Poly& poly);

    // Get all polygons in the scene
    const std::vector<Poly>& getPolygons() const;

    // Get the total number of polygons
    size_t getPolygonCount() const;

    // Apply a transformation to the entire scene
    void applyTransform(const Matrix4& transform);

    // Clear the scene (e.g., before loading a new file)
    void clear();
};

#endif // SCENE_H
