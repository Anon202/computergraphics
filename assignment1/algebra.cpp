#include <math.h>
#include <stdio.h>
#include "algebra.h"

Vector CrossProduct(Vector a, Vector b) {
	Vector v = { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
	return v;
}

float DotProduct(Vector a, Vector b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector Subtract(Vector a, Vector b) {
	Vector v = { a.x-b.x, a.y-b.y, a.z-b.z };
	return v;
}    

Vector Add(Vector a, Vector b) {
	Vector v = { a.x+b.x, a.y+b.y, a.z+b.z };
	return v;
}    

float Length(Vector a) {
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vector Normalize(Vector a) {
	float len = Length(a);
	Vector v = { a.x/len, a.y/len, a.z/len };
	return v;
}

Vector ScalarVecMul(float t, Vector a) {
	Vector b = { t*a.x, t*a.y, t*a.z };
	return b;
}

HomVector MatVecMul(Matrix a, Vector b) {
	HomVector h;
	h.x = b.x*a.e[0] + b.y*a.e[4] + b.z*a.e[8] + a.e[12];
	h.y = b.x*a.e[1] + b.y*a.e[5] + b.z*a.e[9] + a.e[13];
	h.z = b.x*a.e[2] + b.y*a.e[6] + b.z*a.e[10] + a.e[14];
	h.w = b.x*a.e[3] + b.y*a.e[7] + b.z*a.e[11] + a.e[15];
	return h;
}

Vector Homogenize(HomVector h) {
	Vector a;
	if (h.w == 0.0) {
		fprintf(stderr, "Homogenize: w = 0\n");
		a.x = a.y = a.z = 9999999;
		return a;
	}
	a.x = h.x / h.w;
	a.y = h.y / h.w;
	a.z = h.z / h.w;
	return a;
}

Matrix MatMatMul(Matrix a, Matrix b) {
	Matrix c;
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c.e[j*4+i] = 0.0;
			for (k = 0; k < 4; k++)
				c.e[j*4+i] += a.e[k*4+i] * b.e[j*4+k];
		}
	}
	return c;
}

void PrintVector(char const *name, Vector a) {
	printf("%s: %6.5lf %6.5lf %6.5lf\n", name, a.x, a.y, a.z);
}

void PrintHomVector(char const *name, HomVector a) {
	printf("%s: %6.5lf %6.5lf %6.5lf %6.5lf\n", name, a.x, a.y, a.z, a.w);
}

void PrintMatrix(char const *name, Matrix a) { 
	int i,j;

	printf("%s:\n", name);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%6.5lf ", a.e[j*4+i]);
		}
		printf("\n");
	}
}

float radToDeg(float rad) {
    return rad * 180.0/M_PI;
}

float degToRad(float deg) {
    return deg * M_PI/180.0;
}

Matrix rotationMat(char axis, float rad) {
    Matrix r;
    float cr = cos(rad);
    float sr = sin(rad);
    switch (axis) {
        case 'x':
        case 'X':
            r.e[0] = 1.0; r.e[4] = 0.0; r.e[ 8] = 0.0; r.e[12] = 0.0;
            r.e[1] = 0.0; r.e[5] =  cr; r.e[ 9] = -sr; r.e[13] = 0.0;
            r.e[2] = 0.0; r.e[6] =  sr; r.e[10] = cr;  r.e[14] = 0.0;
            r.e[3] = 0.0; r.e[7] = 0.0; r.e[11] = 0.0; r.e[15] = 1.0;
            break;
        case 'y':
        case 'Y':
            r.e[0] =  cr; r.e[4] = 0.0; r.e[ 8] =  sr; r.e[12] = 0.0;
            r.e[1] = 0.0; r.e[5] = 1.0; r.e[ 9] = 0.0; r.e[13] = 0.0;
            r.e[2] = -sr; r.e[6] = 0.0; r.e[10] =  cr; r.e[14] = 0.0;
            r.e[3] = 0.0; r.e[7] = 0.0; r.e[11] = 0.0; r.e[15] = 1.0;
            break;
        case 'z':
        case 'Z':
            r.e[0] =  cr; r.e[4] = -sr; r.e[ 8] = 0.0; r.e[12] = 0.0;
            r.e[1] =  sr; r.e[5] =  cr; r.e[ 9] = 0.0; r.e[13] = 0.0;
            r.e[2] = 0.0; r.e[6] = 0.0; r.e[10] = 1.0; r.e[14] = 0.0;
            r.e[3] = 0.0; r.e[7] = 0.0; r.e[11] = 0.0; r.e[15] = 1.0;
            break;
        default:
            return r;
    }
    return r;
}

Matrix translationMat(Vector x) {
    Matrix r;
    r.e[0] = 1.0; r.e[4] = 0.0; r.e[ 8] = 0.0; r.e[12] = x.x;
    r.e[1] = 0.0; r.e[5] = 1.0; r.e[ 9] = 0.0; r.e[13] = x.y;
    r.e[2] = 0.0; r.e[6] = 0.0; r.e[10] = 1.0; r.e[14] = x.z;
    r.e[3] = 0.0; r.e[7] = 0.0; r.e[11] = 0.0; r.e[15] = 1.0;
    return r;
}

Matrix scaleMat(float sx, float sy, float sz) {
    Matrix s;
    s.e[0] =  sx; s.e[4] = 0.0; s.e[ 8] = 0.0; s.e[12] = 0.0;
    s.e[1] = 0.0; s.e[5] =  sy; s.e[ 9] = 0.0; s.e[13] = 0.0;
    s.e[2] = 0.0; s.e[6] = 0.0; s.e[10] =  sz; s.e[14] = 0.0;
    s.e[3] = 0.0; s.e[7] = 0.0; s.e[11] = 0.0; s.e[15] = 1.0;
    return s;
}

Matrix perspectiveProjMat(float n, float f, float fovdeg, int screenw, int screenh) {
    Matrix p;
    float fov = degToRad(fovdeg);
    float t = n*tan(fov/2);
    float r = (t*screenw)/screenh;
    float b = -t;
    float l = -r;
    p.e[0] = 2*n/(r-l); p.e[4] = 0.0;       p.e[ 8] = (l+r)/(l-r); p.e[12] = 0.0;
    p.e[1] = 0.0;       p.e[5] = 2*n/(t-b); p.e[ 9] = (b+t)/(b-t); p.e[13] = 0.0;
    p.e[2] = 0.0;       p.e[6] = 0.0;       p.e[10] = (f+n)/(n-f); p.e[14] = 2*f*n/(n-f);
    p.e[3] = 0.0;       p.e[7] = 0.0;       p.e[11] = -1.0;         p.e[15] = 0.0;
    return p;
}

/*Matrix parallelProjMat(float n, float f, float fovdeg, int screenw, int screenh) {
    Matrix p;
    float fov = degToRad(fovdeg);
    float t = n*tan(fov/2);
    float r = (t*screenw)/screenh;
    float b = -t;
    float l = -r;
    p.e[0] = 2/(r-l); p.e[4] = 0.0;     p.e[ 8] = 0.0;      p.e[12] = -(r+l)/(r-l);
    p.e[1] = 0.0;     p.e[5] = 2/(t-b); p.e[ 9] = 0.0;      p.e[13] = -(t+b)/(t-b);
    p.e[2] = 0.0;     p.e[6] = 0.0;     p.e[10] = -2/(f-n); p.e[14] = -(n+f)/(f-n);
    p.e[3] = 0.0;     p.e[7] = 0.0;     p.e[11] = 0.0;      p.e[15] = 1.0;
    return p;
}*/

Matrix parallelProjMat(float n, float f, float fovdeg, int screenw, int screenh) {
    Matrix p;
    float fov = degToRad(fovdeg);
    float t = n*tan(fov/2);
    float r = (t*screenw)/screenh;
    p.e[0] = 1/r; p.e[4] = 0.0; p.e[ 8] = 0.0;      p.e[12] = 0.0;
    p.e[1] = 0.0; p.e[5] = 1/t; p.e[ 9] = 0.0;      p.e[13] = 0.0;
    p.e[2] = 0.0; p.e[6] = 0.0; p.e[10] = -2/(f-n); p.e[14] = -(f+n)/(f-n);
    p.e[3] = 0.0; p.e[7] = 0.0; p.e[11] = 0.0;      p.e[15] = 1.0;
    return p;
}

/*Matrix perspectiveMat(float n, float f) {
    Matrix p;
    p.e[0] =   n; p.e[4] = 0.0; p.e[ 8] = 0.0; p.e[12] = 0.0;
    p.e[1] = 0.0; p.e[5] =   n; p.e[ 9] = 0.0; p.e[13] = 0.0;
    p.e[2] = 0.0; p.e[6] = 0.0; p.e[10] = n+f; p.e[14] = -f*n;
    p.e[3] = 0.0; p.e[7] = 0.0; p.e[11] = 1.0; p.e[15] = 0.0;
    return p;
}

Matrix perspectiveProjMat(float n, float f, float fovdeg, int screenw, int screenh) {
    Matrix mOrth = parallelProjMat(n, f, fovdeg, screenw, screenh);
    Matrix p = perspectiveMat(n, f);
    return MatMatMul(p, mOrth);
}

Matrix parallelProjMat(float n, float f, float fovdeg, int screenw, int screenh) {
    Matrix p;
    float fov = degToRad(fovdeg);
    float t = n*tan(fov/2);
    float r = (t*screenw)/screenh;
    float b = -t;
    float l = -r;
    p.e[0] = 2/(r-l); p.e[4] = 0.0;     p.e[ 8] = 0.0;     p.e[12] = -(r+l)/(r-l);
    p.e[1] = 0.0;     p.e[5] = 2/(t-b); p.e[ 9] = 0.0;     p.e[13] = -(t+b)/(t-b);
    p.e[2] = 0.0;     p.e[6] = 0.0;     p.e[10] = 2/(n-f); p.e[14] = -(n+f)/(n-f);
    p.e[3] = 0.0;     p.e[7] = 0.0;     p.e[11] = 0.0;     p.e[15] = 1.0;
    return p;
}*/
