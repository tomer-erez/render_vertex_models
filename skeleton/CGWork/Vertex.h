#ifndef VERTEX_H
#define VERTEX_H

#include "Vector4.h"
#include "Normal.h"

class Vertex : public Vector4 {
private:
    Normal normalFromFile;       // Normal provided from a file
    Normal normalCalculated;     // Normal calculated programmatically
    bool hasNormal;
    bool normalProvidedFromFile; // Flag to indicate if the normal was provided from a file

public:
    // Constructors
    Vertex();
    Vertex(float x, float y, float z, float w);
    Vertex(float x, float y, float z);
    Vertex(const Vector4& position, const Normal& normalFromFile, bool normalProvidedFromFile);
        
    double transperancy = 0;

    // Assignment operator from Vector4
    Vertex& operator=(const Vector4& v);

    // Getters
    const Normal& getNormalFromFile() const;
    const Normal& getNormalCalculated() const;
    bool getHasNormal() const;
    bool isNormalProvidedFromFile() const;

    // Setters
    void setNormalFromFile(const Normal& normal);
    void setNormalCalculated(const Normal& normal);
    void setNormalProvidedFromFile(bool provided);
    void flipNormals();
    void applyTransform(const Matrix4& transform, const Matrix4& normalTransform);

    // Convenience methods
    void updateNormalDirection(const Vector4& direction); // Updates normal direction relative to position
};

#endif // VERTEX_H
