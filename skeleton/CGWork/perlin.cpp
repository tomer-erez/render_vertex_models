#include "perlin.h"
#include <cstdlib> // For std::rand and std::srand

// Define the permutation table
int permutation[PERMUTATION_SIZE * 2];

// Define the gradient table
float grad3[12][3] = {
    {1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
    {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
    {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
};

// Initialize the permutation table
void initializePermutation() {
    static bool initialized = false;
    if (initialized) return;

    // Initialize with values 0 to 255
    std::iota(permutation, permutation + PERMUTATION_SIZE, 0);

    // Shuffle values
    std::random_shuffle(permutation, permutation + PERMUTATION_SIZE);

    // Duplicate for overflow handling
    for (int i = 0; i < PERMUTATION_SIZE; ++i) {
        permutation[PERMUTATION_SIZE + i] = permutation[i];
    }

    initialized = true;
}

// Dot product between gradient and distance vector
float dot(const float* g, float x, float y, float z) {
    return g[0] * x + g[1] * y + g[2] * z;
}

// Fade function
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// 3D Perlin noise function
float perlinNoise3D(float x, float y, float z) {
    // Find unit cube containing the point
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;

    // Relative positions in the unit cube
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    // Compute fade curves for each axis
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // Hash coordinates of the cube corners
    int A = permutation[X] + Y;
    int AA = permutation[A] + Z;
    int AB = permutation[A + 1] + Z;
    int B = permutation[X + 1] + Y;
    int BA = permutation[B] + Z;
    int BB = permutation[B + 1] + Z;

    // Add blended results from all eight corners
    return lerp(w,
        lerp(v,
            lerp(u, dot(grad3[permutation[AA] % 12], x, y, z),
                dot(grad3[permutation[BA] % 12], x - 1, y, z)),
            lerp(u, dot(grad3[permutation[AB] % 12], x, y - 1, z),
                dot(grad3[permutation[BB] % 12], x - 1, y - 1, z))),
        lerp(v,
            lerp(u, dot(grad3[permutation[AA + 1] % 12], x, y, z - 1),
                dot(grad3[permutation[BA + 1] % 12], x - 1, y, z - 1)),
            lerp(u, dot(grad3[permutation[AB + 1] % 12], x, y - 1, z - 1),
                dot(grad3[permutation[BB + 1] % 12], x - 1, y - 1, z - 1))));
}



float fractalNoise(float x, float y, float z, int octaves , float persistence ) {
    float total = 0.0f;     // Final accumulated noise
    float amplitude = 1.0f; // Initial amplitude
    float frequency = 1.0f; // Initial frequency
    float maxValue = 0.0f;  // Used for normalization

    for (int i = 0; i < octaves; ++i) {
        // Accumulate noise with increasing frequency and decreasing amplitude
        total += perlinNoise3D(x * frequency, y * frequency, z * frequency) * amplitude;

        maxValue += amplitude; // Track the max value for normalization
        amplitude *= persistence; // Decrease amplitude for the next octave
        frequency *= 2.0f;        // Increase frequency for the next octave
    }

    // Normalize the result to range [0, 1]
    return total / maxValue;
}
