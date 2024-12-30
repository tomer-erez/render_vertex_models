#pragma once
#ifndef LINEDRAWER_H
#define LINEDRAWER_H

#include <windows.h> // For drawing to the screen
#include "Vector4.h"
#include "Point.h"

class LineDrawer {
public:
    // Bresenham's line-drawing algorithm
    static void DrawLine(Point* oBuffer, size_t width, size_t height, const Vector4& start, const Vector4& end, COLORREF color);
};

#endif // LINEDRAWER_H
