#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

using namespace std;

namespace algebra {

class Vector {
public:
    union {
        float arr[3];
        struct { float x, y, z; };
        struct { float r, g, b; };
    };
    Vector();
    Vector(float x, float y, float z);
    Vector Add(const Vector& v) const;
    Vector Subtract(const Vector& v) const;
    Vector Cross(const Vector& v) const;
    float Dot(const Vector& v) const;
    float Length(void) const;
    Vector& Normalize(void);
    Vector Normalized(void) const;
    Vector ScalarMult(float c) const;
    Vector MultCoordwise(const Vector& v) const;
    void Print(const char *name) const;    
    Vector operator-(void) const;
    Vector operator-(const Vector& rhs) const;
    Vector operator+(const Vector& rhs) const;
    Vector& operator +=(const Vector& v);
    Vector operator*(float f) const;
    friend ostream& operator<<(ostream &o, const Vector& v);
};

typedef Vector Color;

}

#endif
