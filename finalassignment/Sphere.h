#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Vec3.h"
#include "Ray.h"

class Sphere {
public:
    Vec3f c;
    float r;
    Vec3f color;
public:
    Sphere(const Vec3f & cen, float rad, const Color& color);

    bool Hit(const Ray & r, HitRec & rec) const;
    void ComputeSurfaceHitFields(const Ray & r, HitRec & rec) const;

};

#endif
