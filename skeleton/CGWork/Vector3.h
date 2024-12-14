#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

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
    double dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    double magnitude() const;
    Vector3 normalize() const;
};

#endif // VECTOR3_H
