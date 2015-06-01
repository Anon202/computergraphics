#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Ray.h"

typedef struct _Material {
    Vector ambient, diffuse, specular;
    float shininess;
    bool reflective;
    bool transparency;
    float refractionIndex;
} Material;

class Shape {
public:
    Material material;
    explicit Shape(Material mat) : material(mat) {}
    virtual bool Hit(const Ray &r, HitRec & rec) const = 0;
};

#endif