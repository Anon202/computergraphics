#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

using namespace std;

namespace algebra {

class Vector {
public:
    float x, y, z;
    Vector();
    Vector(float x, float y, float z);
    Vector Add(Vector v);
    Vector Subtract(Vector v);
    Vector Cross(Vector v);
    float Dot(Vector v);
    float Length();
    Vector Normalized();
    Vector ScalarMult(float c);
    void Print(const char *name);    
    Vector operator-();
    Vector operator-(const Vector& rhs);
    Vector operator+(const Vector& rhs);
    Vector& operator +=(const Vector& v);
    friend ostream& operator<<(ostream &o, const Vector& v);
};

}

#endif
