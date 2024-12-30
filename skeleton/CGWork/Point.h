#ifndef POINT_H
#define POINT_H

#include "Vector4.h"
#include <Windows.h> // For COLORREF type

class Point : public Vector4 {
private:
    COLORREF color; // Color attribute

public:
    // Constructors
    Point();
    Point(float x, float y, float z, float w = 1.0f, COLORREF color = RGB(255, 255, 255));
    Point(const Vector4& vector, COLORREF color = RGB(255, 255, 255));

    // Set and get color
    void setColor(COLORREF newColor);
    COLORREF getColor() const;
};

#endif // POINT_H
