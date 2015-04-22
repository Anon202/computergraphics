#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "Matrix.h"
#include "utilities.h"

namespace algebra {
    Matrix::Matrix() {
        for (int i = 0; i < 16; i++) {
            e[i] = 0.0;
        }
    }

    Matrix::Matrix(float e[16]) {
        memcpy(this->e, e, sizeof(float) * 16);
    }

    float Matrix::get(int i, int j) const {
        return e[j*4 + i];
    }

    void Matrix::set(int i, int j, float x) {
        if (i >= 4 || i < 0 || j >= 4 || j < 0) {
            // TODO: throw exception
            return;
        }
        e[j*4 + i] = x;
    }
    
    Matrix Matrix::multiply(Matrix m) {
        Matrix r = Matrix();
	    for (int i = 0; i < 4; i++) {
		    for (int j = 0; j < 4; j++) {
			    for (int k = 0; k < 4; k++) {
				    r.e[j*4+i] += this->e[k*4+i] * m.e[j*4+k];
                }
		    }
	    }
	    return r;
    }

    Vector Matrix::multiply(Vector v) {
        float x = get(0, 0)*v.x + get(0, 1)*v.y + get(0, 2)*v.z;
        float y = get(1, 0)*v.x + get(1, 1)*v.y + get(1, 2)*v.z;
        float z = get(2, 0)*v.x + get(2, 1)*v.y + get(2, 2)*v.z;
        return Vector(x, y, z);
    }
    
    Matrix Matrix::multiply(float c) {
        Matrix r = Matrix();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) { r.set(i, j, this->get(i, j) * c);
            }
        }
        return r;
    }
    
    HomVector Matrix::multiplyH(Vector v) {
        float x = get(0, 0)*v.x + get(0, 1)*v.y + get(0, 2)*v.z + get(0, 3);
        float y = get(1, 0)*v.x + get(1, 1)*v.y + get(1, 2)*v.z + get(1, 3);
        float z = get(2, 0)*v.x + get(2, 1)*v.y + get(2, 2)*v.z + get(2, 3);
        float w = get(3, 0)*v.x + get(3, 1)*v.y + get(3, 2)*v.z + get(3, 3);
        return HomVector(x, y, z, w);
    }
    
    void Matrix::print(const char *name) {
	    cout << name << ": " << endl << *this << endl;
    }
    
    Matrix Matrix::operator*(const Matrix& m) {
        return this->multiply(m);
    }
    
    Vector Matrix::operator*(const Vector& v) {
        return this->multiply(v);
    }

    Matrix& Matrix::operator=(const Matrix& rhs) {
        if (this == &rhs) {
            return *this;
        }
        memcpy(this->e, rhs.e, sizeof(float) * 16);
        return *this;
    }

    ostream& operator<<(ostream &o, const Matrix& m) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                o << m.get(i, j) << " ";
            }
            o << endl;
        }
        return o;
    }
    
    Matrix Matrix::rotation(char axis, float rad) {
        Matrix m = Matrix();
        float cr = cos(rad);
        float sr = sin(rad);
        switch (axis) {
            case 'x':
            case 'X':
                m.e[0] = 1.0; m.e[4] = 0.0; m.e[ 8] = 0.0; m.e[12] = 0.0;
                m.e[1] = 0.0; m.e[5] =  cr; m.e[ 9] = -sr; m.e[13] = 0.0;
                m.e[2] = 0.0; m.e[6] =  sr; m.e[10] = cr;  m.e[14] = 0.0;
                m.e[3] = 0.0; m.e[7] = 0.0; m.e[11] = 0.0; m.e[15] = 1.0;
                break;
            case 'y':
            case 'Y':
                m.e[0] =  cr; m.e[4] = 0.0; m.e[ 8] =  sr; m.e[12] = 0.0;
                m.e[1] = 0.0; m.e[5] = 1.0; m.e[ 9] = 0.0; m.e[13] = 0.0;
                m.e[2] = -sr; m.e[6] = 0.0; m.e[10] =  cr; m.e[14] = 0.0;
                m.e[3] = 0.0; m.e[7] = 0.0; m.e[11] = 0.0; m.e[15] = 1.0;
                break;
            case 'z':
            case 'Z':
                m.e[0] =  cr; m.e[4] = -sr; m.e[ 8] = 0.0; m.e[12] = 0.0;
                m.e[1] =  sr; m.e[5] =  cr; m.e[ 9] = 0.0; m.e[13] = 0.0;
                m.e[2] = 0.0; m.e[6] = 0.0; m.e[10] = 1.0; m.e[14] = 0.0;
                m.e[3] = 0.0; m.e[7] = 0.0; m.e[11] = 0.0; m.e[15] = 1.0;
                break;
            default:
                // TODO: throw exception
                return m;
        }
        return m;
    }
    
    Matrix Matrix::translation(Vector x) {
        Matrix m = Matrix();
        m.e[0] = 1.0; m.e[4] = 0.0; m.e[ 8] = 0.0; m.e[12] = x.x;
        m.e[1] = 0.0; m.e[5] = 1.0; m.e[ 9] = 0.0; m.e[13] = x.y;
        m.e[2] = 0.0; m.e[6] = 0.0; m.e[10] = 1.0; m.e[14] = x.z;
        m.e[3] = 0.0; m.e[7] = 0.0; m.e[11] = 0.0; m.e[15] = 1.0;
        return m;
    }
    
    Matrix Matrix::scale(Vector s) {
        Matrix m = Matrix();
        m.e[0] = s.x; m.e[4] = 0.0; m.e[ 8] = 0.0; m.e[12] = 0.0;
        m.e[1] = 0.0; m.e[5] = s.y; m.e[ 9] = 0.0; m.e[13] = 0.0;
        m.e[2] = 0.0; m.e[6] = 0.0; m.e[10] = s.z; m.e[14] = 0.0;
        m.e[3] = 0.0; m.e[7] = 0.0; m.e[11] = 0.0; m.e[15] = 1.0;
        return m;
    }
    
    Matrix Matrix::perspectiveProj(float n, float f, float fovdeg, float aspect) {
        Matrix m = Matrix();
        float fov = degToRad(fovdeg);
        float t = n*tan(fov/2);
        float r = t*aspect;;
        float b = -t;
        float l = -r;
        m.e[0] = 2*n/(r-l); m.e[4] = 0.0;       m.e[ 8] = (r+l)/(r-l); m.e[12] = 0.0;
        m.e[1] = 0.0;       m.e[5] = 2*n/(t-b); m.e[ 9] = (t+b)/(t-b); m.e[13] = 0.0;
        m.e[2] = 0.0;       m.e[6] = 0.0;       m.e[10] = (n+f)/(n-f); m.e[14] = 2*f*n/(n-f);
        m.e[3] = 0.0;       m.e[7] = 0.0;       m.e[11] = -1.0;        m.e[15] = 0.0;
        return m;
    }
    
    Matrix Matrix::parallelProj(float t, float r, float b, float l, int n, int f) {
        Matrix m = Matrix();
        m.e[0] = 2/(r-l); m.e[4] = 0.0;     m.e[ 8] = 0.0;      m.e[12] = -(r+l)/(r-l);
        m.e[1] = 0.0;     m.e[5] = 2/(t-b); m.e[ 9] = 0.0;      m.e[13] = -(t+b)/(t-b);
        m.e[2] = 0.0;     m.e[6] = 0.0;     m.e[10] = -2/(n-f); m.e[14] = -(f+n)/(f-n);
        m.e[3] = 0.0;     m.e[7] = 0.0;     m.e[11] = 0.0;      m.e[15] = 1.0;
        return m;
    }
   
    /* Rodrigues' Rotation */
    Matrix Matrix::rotationAroundAxis(Vector v, float rad) {
        Matrix m = Matrix();
        Vector u = v.normalized();
        float c = cos(rad);
        float s = sin(rad);
        m.e[ 0] = c + u.x*u.x*(1 - c);
        m.e[ 1] = u.y*u.x*(1 - c) + u.z*s;
        m.e[ 2] = u.z*u.x*(1 - c) - u.y*s;
        m.e[ 3] = 0.0;

        m.e[ 4] = u.x*u.y*(1 - c) - u.z*s;
        m.e[ 5] = c + u.y*u.y*(1 - c);
        m.e[ 6] = u.z*u.y*(1 - c) + u.x*s;
        m.e[ 7] = 0.0;
        
        m.e[ 8] = u.x*u.z*(1 - c) + u.y*s;
        m.e[ 9] = u.y*u.z*(1 - c) - u.x*s;
        m.e[10] = c + u.z*u.z*(1 - c);
        m.e[11] = 0.0;
 
        m.e[12] = 0.0;
        m.e[13] = 0.0;
        m.e[14] = 0.0;
        m.e[15] = 1.0;

        return m;
    }
}
