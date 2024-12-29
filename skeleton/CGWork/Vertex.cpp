#include "Vertex.h"
const double F = 16.0;


// Default constructor
Vertex::Vertex() : Vector4(), normalFromFile(Normal()), normalCalculated(Normal()), hasNormal(false), normalProvidedFromFile(false) {}

// Constructor with position (4 arguments)
Vertex::Vertex(float x, float y, float z, float w)
    : Vector4(x, y, z, w), normalFromFile(Normal()), normalCalculated(Normal()), hasNormal(false), normalProvidedFromFile(false) {}

// Constructor with position (3 arguments, defaults w to 1.0f)
Vertex::Vertex(float x, float y, float z)
    : Vector4(x, y, z, 1.0f), normalFromFile(Normal()), normalCalculated(Normal()), hasNormal(false), normalProvidedFromFile(false) {}

// Constructor with normal and flag
Vertex::Vertex(const Vector4& position, const Normal& normalFromFile, bool normalProvidedFromFile)
    : Vector4(position), normalFromFile(normalFromFile), normalCalculated(Normal()), hasNormal(true), normalProvidedFromFile(normalProvidedFromFile) {}

// Assignment operator from Vector4
Vertex& Vertex::operator=(const Vector4& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

// Getters
const Normal& Vertex::getNormalFromFile() const {
    return normalFromFile;
}

const Normal& Vertex::getNormalCalculated() const {
    return normalCalculated;
}

bool Vertex::getHasNormal() const {
    return hasNormal;
}

bool Vertex::isNormalProvidedFromFile() const {
    return normalProvidedFromFile;
}

// Setters
void Vertex::setNormalFromFile(const Normal& normal) {
    normalFromFile = normal;
    hasNormal = true;
}

void Vertex::setNormalCalculated(const Normal& normal) {
    normalCalculated = normal;
    hasNormal = true;
}

void Vertex::setNormalProvidedFromFile(bool provided) {
    normalProvidedFromFile = provided;
}

// Update the normal direction relative to the vertex position
void Vertex::updateNormalDirection(const Vector4& direction) {
    normalCalculated = Normal(*this, *this + direction.normalize());
    hasNormal = true;
}


void Vertex::applyTransform(const Matrix4& transform, const Matrix4& normalTransform) {
    // Transform the vertex position
    *this = transform.transform(*this);

    // Transform normals if the vertex has a normal
    const Vector4 direction = normalCalculated.end - normalCalculated.start;
    const Vector4 transformedDirection = normalTransform.transform(direction).normalize() * F;

    // Transform the start and end points of the normal
    const Vector4 transformedStart = transform.transform(normalCalculated.start);
    normalCalculated = Normal(transformedStart, transformedStart + transformedDirection);

    // Transform the normal from file if it exists
    const Vector4 fileDirection = normalFromFile.end - normalFromFile.start;
    const Vector4 transformedFileDirection = normalTransform.transform(fileDirection).normalize() * F;

    const Vector4 transformedFileStart = transform.transform(normalFromFile.start);
    normalFromFile = Normal(transformedFileStart, transformedFileStart + transformedFileDirection);
    
}