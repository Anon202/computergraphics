#include "Sphere.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace algebra;

Sphere::Sphere(const Vector& cen, float rad) : c(cen), r(rad) {
}

bool Sphere::Hit(const Ray &r, HitRec &rec) const {
    Vector v = r.o - this->c;
    float a = r.d.Dot(r.d);
    float b = 2*v.Dot(r.d);
    float c = v.Dot(v) - this->r*this->r;
    float d = b*b - 4*a*c;
    if (d < 0) {
        rec.anyHit = false;
        return false;
    }
    float t1 = (-b - sqrt(d))/(2*a);
    float t2 = (-b + sqrt(d))/(2*a);
    if (t1 >= 0) {
        rec.tHit = t1;
    } else if (t2 >= 0) {
        rec.tHit = t2;
    } else {
        rec.anyHit = false;
        return false;
    }
    rec.anyHit = true;
    rec.p = r.o + r.d * rec.tHit;
    rec.n = (rec.p - this->c).Normalized();
    return true;
}
