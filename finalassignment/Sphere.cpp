
#include "Sphere.h"

Sphere::Sphere(const Vec3f & cen, float rad) : c(cen), r(rad) {
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
    rec.anyHit = true;
    return true;
}


void Sphere::ComputeSurfaceHitFields(const Ray & r, HitRec & rec) const {
    rec.p = r.o + r.d * rec.tHit;
    rec.n = (rec.p - this->c).normalize();
}
