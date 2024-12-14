#pragma once
#ifndef LINEDRAWER_H
#define LINEDRAWER_H

#include <windows.h> // For drawing to the screen
#include "Vector3.h"

class LineDrawer {
public:
    // Bresenham's line-drawing algorithm
    static void DrawLine(HDC hdc, const Vector3& start, const Vector3& end, COLORREF color);
};

#endif // LINEDRAWER_H
