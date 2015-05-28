#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

using namespace std;

namespace algebra {

class Vector {
public:
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };
    Vector();
    Vector(float x, float y, float z);
    Vector Add(Vector v) const;
    Vector Subtract(Vector v) const;
    Vector Cross(Vector v) const;
    float Dot(Vector v) const;
    float Length() const;
    Vector Normalized() const;
    Vector ScalarMult(float c) const;
    void Print(const char *name) const;    
    Vector operator-() const;
    Vector operator-(const Vector& rhs) const;
    Vector operator+(const Vector& rhs) const;
    Vector& operator +=(const Vector& v);
    Vector operator*(float f) const;
    friend ostream& operator<<(ostream &o, const Vector& v);
};

typedef Vector Color;

}

#endif
