#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

using namespace std;

namespace algebra {

class Vector {
private:
public:
    float x, y, z;
    Vector();
    Vector(float x, float y, float z);
    Vector add(Vector v);
    Vector subtract(Vector v);
    Vector cross(Vector v);
    float dot(Vector v);
    float length();
    Vector normalized();
    Vector scalarMult(float c);
    void print(const char *name);    
    Vector operator-(const Vector& rhs);
    Vector operator+(const Vector& rhs);
    Vector& operator +=(const Vector& v);
    friend ostream& operator<<(ostream &o, const Vector& v);
};

}

#endif
