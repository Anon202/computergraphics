#include "Plane.h"
#include <cmath>

#define min3(a, b, c) min(a, min(b, c))
#define max3(a, b, c) max(a, max(b, c))
#define EPSILON 0.001f

Plane::Plane(Vector p1, Vector p2, Vector p3, Material mat) : Shape(mat) {
    Vector ab = p2 - p1;
    Vector ac = p3 - p1;
    this->normal = ab.Cross(ac).Normalized();
    this->d = -this->normal.Dot(p1);
    this->point = p1;
}

bool Plane::Hit(const Ray &r, HitRec &rec) const {
    float dDotNormal = r.d.Dot(this->normal);
    if (dDotNormal >= -EPSILON && dDotNormal <= EPSILON) {
        rec.anyHit = false;
        return false;
    }
    float t = (this->point - r.o).Dot(this->normal)/dDotNormal;
    rec.anyHit = false;
    if (t > EPSILON) {
        rec.tHit = t;
        rec.anyHit = true;
        rec.n = this->normal;
        rec.p = r.o + r.d * rec.tHit;
    }
    return rec.anyHit;
}
