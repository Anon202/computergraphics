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
    Vec3f v = r.o - this->c;
    float b = 2*v.dot(r.d);
    float c = v.dot(v) - this->r*this->r;
    float d = b*b - 4*c;
    if (d < 0) {
        rec.anyHit = false;
        return false;
    }
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
