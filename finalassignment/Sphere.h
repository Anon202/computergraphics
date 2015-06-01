#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../algebra/Vector.h"
#include "Shape.h"

using namespace algebra;

class Sphere : public Shape {
public:
    Vector c;
    float r;

public:
    Sphere(const Vector & cen, float rad, Material mat);
    bool Hit(const Ray & r, HitRec & rec) const;
};

#endif
