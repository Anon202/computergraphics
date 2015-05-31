#ifndef _SPHERE_H
#define _SPHERE_H

#include <vector>
#include "Sphere.h"
#include "Light.h"
#include "../algebra/Vector.h"

using namespace std;
using namespace algebra;

class Scene {
public:
    vector<Sphere> spheres;
    vector<Light> lights;
    Scene(void);
    void Add(const Sphere & sphere);
    void Add(const Light & light);
    void Load(char * fileName);
};

#endif
