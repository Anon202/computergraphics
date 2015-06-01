#ifndef _SPHERE_H
#define _SPHERE_H

#include <vector>
#include "Shape.h"
#include "Light.h"
#include "../algebra/Vector.h"

using namespace std;
using namespace algebra;

class Scene {
public:
    vector<Shape*> shapes;
    vector<Light> lights;
    Scene(void);
    void Add(Shape* shape);
    void Add(const Light & light);
    void Load(char * fileName);
};

#endif
