#include "LineDrawer.h"
//the midpoint algorithm
void LineDrawer::DrawLine(Point* oBuffer, size_t width, size_t height, const Vector4& start, const Vector4& end, COLORREF color) {
    int x1 = static_cast<int>(start.x);
    int y1 = static_cast<int>(start.y);
    int x2 = static_cast<int>(end.x);
    int y2 = static_cast<int>(end.y);

    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < static_cast<int>(width) && y1 >= 0 && y1 < static_cast<int>(height)) {
            size_t index = y1 * width + x1;
            oBuffer[index] = Point(static_cast<float>(x1), static_cast<float>(y1), 0.0f, 1.0f, color);
        }

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

