#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../algebra/Vector.h"
#include "Shape.h"
#include "textures/SphereTexture.h"

using namespace algebra;

class Sphere : public Shape {
private:
    Vector c;
    float r;
    SphereTexture* texture;

public:
    TextureConf colorConf = TextureConf::ONLY_COLOR;
    Sphere(const Vector & cen, float rad, Material mat);
    Sphere(const Vector & cen, float rad, Material mat, const char* texturefile);
    bool Hit(const Ray & r, HitRec & rec) const;
    Color GetColor(Vector position) const;
};

#endif
