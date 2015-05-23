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
    float Get(int i, int j) const;
    void Set(int i, int j, float x);
    Matrix Transposed();
    Matrix Multiply(Matrix m);
    Vector Multiply(Vector v);
    Matrix Multiply(float c);
    HomVector MultiplyH(Vector v);
    void Print(const char *name);    
    Matrix operator*(const Matrix& m);
    Vector operator*(const Vector& v);
    Matrix& operator=(const Matrix& rhs);
    friend ostream& operator<<(ostream &o, const Matrix& m);        
    static Matrix Rotation(char axis, float rad);
    static Matrix Translation(Vector x);
    static Matrix Scale(Vector s);
    static Matrix PerspectiveProj(float n, float f, float fovdeg, float aspect);
    static Matrix ParallelProj(float t, float r, float b, float l, int n, int f);
    static Matrix RotationAroundAxis(Vector v, float rad);
};

}

#endif
