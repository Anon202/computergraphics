#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vector.h"
#include "HomVector.h"
#include <iostream>

using namespace std;

namespace algebra {

class Matrix {
public:
    float e[16];
    Matrix();
    Matrix(float e[16]);
    float get(int i, int j) const;
    void set(int i, int j, float x);
    Matrix multiply(Matrix m);
    Vector multiply(Vector v);
    Matrix multiply(float c);
    HomVector multiplyH(Vector v);
    void print(const char *name);    
    Matrix operator*(const Matrix& m);
    Vector operator*(const Vector& v);
    Matrix& operator=(const Matrix& rhs);
    friend ostream& operator<<(ostream &o, const Matrix& m);        
    static Matrix rotation(char axis, float rad);
    static Matrix translation(Vector x);
    static Matrix scale(Vector s);
    static Matrix perspectiveProj(float n, float f, float fovdeg, float aspect);
    static Matrix parallelProj(float t, float r, float b, float l, int n, int f);
    static Matrix rotationAroundAxis(Vector v, float rad);
};

}

#endif
