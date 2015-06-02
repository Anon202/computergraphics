#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../algebra/Vector.h"
#include "Shape.h"
#include "textures/SphereTexture.h"

using namespace algebra;

enum class SphereConf {
    TEXTURE_AND_COLOR,
    ONLY_COLOR,
    ONLY_TEXTURE
};

class Sphere : public Shape {
private:
    Vector c;
    float r;
    SphereTexture* texture;

public:
    SphereConf colorConf = SphereConf::ONLY_COLOR;
    Sphere(const Vector & cen, float rad, Material mat);
    Sphere(const Vector & cen, float rad, Material mat, const char* texturefile);
    bool Hit(const Ray & r, HitRec & rec) const;
    void Texture(SphereTexture* texture);
    SphereTexture* Texture() const;
    Color GetColor(Vector position) const;
};

#endif
