#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>
#include <algorithm> // For std::min, std::max

class Vector3 {
public:
    double x, y, z;

    // Constructors
    Vector3();
    Vector3(double x, double y, double z);

    // Vector operations
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(double scalar) const;
    Vector3 operator/(double scalar) const;
    double dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    double magnitude() const;
    Vector3 normalize() const;

    // Utility functions
    double distance(const Vector3& v) const;
    void set(double newX, double newY, double newZ);
    void updateMin(const Vector3& v);
    void updateMax(const Vector3& v);
    static Vector3 zero();

    // Angle between vectors
    double angle(const Vector3& v) const;

    // Equality and inequality
    bool operator==(const Vector3& v) const;
    bool operator!=(const Vector3& v) const;

    // Debugging
    void print() const;
};

#endif // VECTOR3_H
