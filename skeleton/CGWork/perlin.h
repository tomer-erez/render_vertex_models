#pragma once

#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

static const int PERMUTATION_SIZE = 256; // Define PERMUTATION_SIZE here

// Permutation table
extern int permutation[];

// Gradient vectors (unit vectors)
extern float grad3[12][3];

// Function declarations
void initializePermutation();
float dot(const float* g, float x, float y, float z);
float fade(float t);
float lerp(float a, float b, float t);
float perlinNoise3D(float x, float y, float z);
