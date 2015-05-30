#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../algebra/Vector.h"
#include "Ray.h"

using namespace algebra;

typedef struct _Material {
    Vector ambient, diffuse, specular;
    float shininess;
    bool reflective;
    bool transparency;
    float refractionIndex;
} Material;

class Sphere {
public:
    Vector c;
    float r;

public:
    Material material;
    Sphere(const Vector & cen, float rad, Material mat);
    bool Hit(const Ray & r, HitRec & rec) const;
};

#endif
