#ifndef ANTIALIASING_H
#define ANTIALIASING_H
#include "Point.h"


void applyBoxFilter(Point* buffer, int width, int height, int x, int y, int kernelSize);
void applyTriangleFilter(Point* buffer, int width, int height, int x, int y, int kernelSize);
void applyGaussianFilter(Point* buffer, int width, int height, int x, int y, int kernelSize);
void applySincFilter(Point* buffer, int width, int height, int x, int y, int kernelSize);

class AntiAliasing
{
};

#endif // ANTIALIASING_H
