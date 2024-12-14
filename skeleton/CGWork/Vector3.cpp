#include "Vector3.h"

// Default constructor
Vector3::Vector3() : x(0), y(0), z(0) {}

// Constructor with values
Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

// Vector addition
Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

// Vector subtraction
Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

// Scalar multiplication
Vector3 Vector3::operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Dot product
double Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// Cross product
Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

// Magnitude of the vector
double Vector3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Normalize the vector
Vector3 Vector3::normalize() const {
    double mag = magnitude();
    return Vector3(x / mag, y / mag, z / mag);
}
