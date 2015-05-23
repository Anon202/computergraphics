#ifndef _SPHERE_H
#define _SPHERE_H

#include <vector>
#include "Sphere.h"

using namespace std;

class Scene {
public:
    vector<Sphere> spheres;
    Scene(void);
    void Add(const Sphere & s);
    void Load(char * fileName);
};

#endif
