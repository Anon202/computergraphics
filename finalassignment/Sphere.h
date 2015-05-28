#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../algebra/Vector.h"
#include "Ray.h"

using namespace algebra;

class Sphere {
public:
    Vector c;
    float r;

public:
    Vector ambient, diffuse, specular;
    float shininess;
    Sphere(const Vector & cen, float rad);
    bool Hit(const Ray & r, HitRec & rec) const;
};

#endif
