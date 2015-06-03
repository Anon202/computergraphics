#ifndef _PLANE_H_
#define _PLANE_H_

#include "Shape.h"
#include "Ray.h"
#include "../algebra/Vector.h"

using namespace algebra;

class Plane : public Shape {
private:
    Vector normal, point;
    float d;

public:
    Plane(Vector p1, Vector p2, Vector p3, Material mat);
    bool Hit(const Ray & r, HitRec & rec) const;
};

#endif
