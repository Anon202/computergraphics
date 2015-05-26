#include "Sphere.h"
#include <cmath>
#include <iostream>

using namespace std;

Sphere::Sphere(const Vec3f & cen, float rad, Color color) : c(cen), r(rad) {
    this->color.r = color.r;
    this->color.g = color.g;
    this->color.b = color.b;
}

bool Sphere::Hit(const Ray &r, HitRec & rec) const {
    Vec3f v = this->c - r.o;
    float s = v.dot(r.d);
    float vLenSq = v.dot(v);
    float radSq = this->r * this->r;
    if (s < 0 && vLenSq > radSq) {
        return false;
    }
    float mSq = vLenSq - s * s;
    if (mSq > radSq) {
        return false;
    }
    float b = 2*s;
    float c = v.dot(v) - radSq;
    float d = b*b - 4*c;
    float t1 = (-b + sqrt(d))/2;
    float t2 = (-b - sqrt(d))/2;
    rec.tHit = (t1 > 0)? t1 : t2;
    rec.anyHit = true;
    return true;
}


void Sphere::ComputeSurfaceHitFields(const Ray & r, HitRec & rec) const {
    rec.p = r.o + r.d * rec.tHit;
    rec.n = (rec.p - this->c).normalize();
}
