#ifndef _SPHERE_H
#define _SPHERE_H

#include <vector>
#include "Sphere.h"

using namespace std;

typedef struct _Light {
    Vec3f position;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
} Light;

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
