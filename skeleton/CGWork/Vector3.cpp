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

// Scalar division
Vector3 Vector3::operator/(double scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
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
    return mag > 0 ? *this / mag : Vector3();
}

// Distance between two vectors
double Vector3::distance(const Vector3& v) const {
    return (*this - v).magnitude();
}

// Set coordinates
void Vector3::set(double newX, double newY, double newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

// Update minimum values
void Vector3::updateMin(const Vector3& v) {
    x = std::min(x, v.x);
    y = std::min(y, v.y);
    z = std::min(z, v.z);
}

// Update maximum values
void Vector3::updateMax(const Vector3& v) {
    x = std::max(x, v.x);
    y = std::max(y, v.y);
    z = std::max(z, v.z);
}

// Zero vector
Vector3 Vector3::zero() {
    return Vector3(0.0, 0.0, 0.0);
}

// Angle between two vectors
double Vector3::angle(const Vector3& v) const {
    double dotProduct = dot(v);
    double magnitudes = magnitude() * v.magnitude();
    return magnitudes > 0 ? std::acos(dotProduct / magnitudes) : 0.0;
}

// Equality comparison
bool Vector3::operator==(const Vector3& v) const {
    return x == v.x && y == v.y && z == v.z;
}

// Inequality comparison
bool Vector3::operator!=(const Vector3& v) const {
    return !(*this == v);
}

// Debugging: Print vector values
void Vector3::print() const {
    std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
}
