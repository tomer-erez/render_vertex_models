#include "Point.h"

// Default constructor
Point::Point() : Vector4(), color(RGB(255, 255, 255)) {} // Default white color

// Constructor with coordinates and color
Point::Point(float x, float y, float z, float w, COLORREF color)
    : Vector4(x, y, z, w), color(color) {}

// Constructor with Vector4 and color
Point::Point(const Vector4& vector, COLORREF color)
    : Vector4(vector), color(color) {}

// Set the color
void Point::setColor(COLORREF newColor) {
    color = newColor;
}

// Get the color
COLORREF Point::getColor() const {
    return color;
}
