#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Vec3.h"
#include "Ray.h"

class Sphere {
public:
    Vec3f c;
    float r;

public:
    Vec3f ambient, diffuse, specular;
    float shininess;
    Sphere(const Vec3f & cen, float rad);
    bool Hit(const Ray & r, HitRec & rec) const;
};

#endif
