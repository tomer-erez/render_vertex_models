#ifndef ANTIALIASING_H
#define ANTIALIASING_H
#include "Point.h"
#include <string>


void applyBoxFilter(Point* buffer, int width, int height,  int kernelSize);
void applyTriangleFilter(Point* buffer, int width, int height, int kernelSize);
void applyGaussianFilter(Point* buffer, int width, int height, int kernelSize);
void applySincFilter(Point* buffer, int width, int height, int kernelSize);
void applyAntiAliasingByName(Point* buffer, int width, int height, int kernelSize, const std::string& filterName);

class AntiAliasing
{
};

#endif // ANTIALIASING_H
