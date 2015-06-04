#include "Sphere.h"
#include "Shape.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace algebra;

Sphere::Sphere(const Vector& cen, float rad, Material mat) : Shape(mat), c(cen), r(rad) {
}

Sphere::Sphere(const Vector& cen, float rad, Material mat, const char* texturefile) :
    Shape(mat), c(cen), r(rad) {
    this->texture = new SphereTexture(texturefile, this->r, this->c);
}

bool Sphere::Hit(const Ray &r, HitRec &rec) const {
    Vector v = r.o - this->c;
    float a = r.d.Dot(r.d);
    float b = 2*v.Dot(r.d);
    float c = v.Dot(v) - this->r*this->r;
    float d = b*b - 4*a*c;
    if (d < 0) {
        rec.anyHit = false;
        return false;
    }
    float t1 = (-b - sqrt(d))/(2*a);
    float t2 = (-b + sqrt(d))/(2*a);
    if (t1 >= 0) {
        rec.tHit = t1;
    } else if (t2 >= 0) {
        rec.tHit = t2;
    } else {
        rec.anyHit = false;
        return false;
    }
    rec.anyHit = true;
    rec.p = r.o + r.d * rec.tHit;
            
    rec.n = (rec.p - this->c).Normalized();
        
    return true;
}

Color Sphere::GetColor(Vector position) const {
    if (this->colorConf == TextureConf::ONLY_COLOR || this->texture == NULL) {
        return this->material.ambient;
    } else if (this->colorConf == TextureConf::ONLY_TEXTURE) {
        MappedCoords coords = this->texture->MapObjectCoords(position);
        return this->texture->GetColor(coords.u, coords.v);
    } else {
        MappedCoords coords = this->texture->MapObjectCoords(position);
        return this->material.ambient.MultCoordwise(this->texture->GetColor(coords.u, coords.v));
    }
}
