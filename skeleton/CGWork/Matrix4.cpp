#include "Matrix4.h"
#include "Vector3.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

// Constructor: Initializes to identity matrix
Matrix4::Matrix4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = (i == j) ? 1.0 : 0.0;
}

// Matrix multiplication
Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

// Transform a Vector3
Vector3 Matrix4::transform(const Vector3& v) const {
    return Vector3(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]
    );
}

// Translation matrix
Matrix4 Matrix4::translate(double dx, double dy, double dz) {
    Matrix4 mat;
    mat.m[0][3] = dx;
    mat.m[1][3] = dy;
    mat.m[2][3] = dz;
    return mat;
}

// Scaling matrix
Matrix4 Matrix4::scale(double sx, double sy, double sz) {
    Matrix4 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    return mat;
}

// Rotation matrix around X-axis
Matrix4 Matrix4::rotateX(double angle) {
    Matrix4 mat;
    double cosA = cos(angle), sinA = sin(angle);
    mat.m[1][1] = cosA;
    mat.m[1][2] = -sinA;
    mat.m[2][1] = sinA;
    mat.m[2][2] = cosA;
    return mat;
}

// Rotation matrix around Y-axis
Matrix4 Matrix4::rotateY(double angle) {
    Matrix4 mat;
    double cosA = cos(angle), sinA = sin(angle);
    mat.m[0][0] = cosA;
    mat.m[0][2] = sinA;
    mat.m[2][0] = -sinA;
    mat.m[2][2] = cosA;
    return mat;
}

// Rotation matrix around Z-axis
Matrix4 Matrix4::rotateZ(double angle) {
    Matrix4 mat;
    double cosA = cos(angle), sinA = sin(angle);
    mat.m[0][0] = cosA;
    mat.m[0][1] = -sinA;
    mat.m[1][0] = sinA;
    mat.m[1][1] = cosA;
    return mat;
}

// Orthographic projection matrix
Matrix4 Matrix4::orthographic(double left, double right, double bottom, double top, double near, double far) {
    Matrix4 mat;
    mat.m[0][0] = 2.0 / (right - left);
    mat.m[1][1] = 2.0 / (top - bottom);
    mat.m[2][2] = -2.0 / (far - near);
    mat.m[0][3] = -(right + left) / (right - left);
    mat.m[1][3] = -(top + bottom) / (top - bottom);
    mat.m[2][3] = -(far + near) / (far - near);
    return mat;
}

// Perspective projection matrix
Matrix4 Matrix4::perspective(double fov, double aspect, double near, double far, double d) {
    Matrix4 mat;
    double tanHalfFOV = tan(fov / 2.0);
    mat.m[0][0] = 1.0 / (aspect * tanHalfFOV);
    mat.m[1][1] = 1.0 / tanHalfFOV;
    mat.m[2][2] = -(far + near) / (far - near);
    mat.m[2][3] = -(2.0 * far * near) / (far - near);
    mat.m[3][2] = -1.0;
    mat.m[3][3] = d; // Modify perspective ratio using d
    return mat;
}

// Modify perspective parameter 'd'
void Matrix4::setPerspectiveD(double d) {
    m[3][3] = d;
}

// Inverse of the matrix (simplified placeholder)
Matrix4 Matrix4::inverse() const {
    throw std::runtime_error("Matrix inversion is not implemented yet.");
}

// Print the matrix (for debugging)
void Matrix4::print() const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
