#ifndef _LIGHT_H
#define _LIGHT_H

#include <random>
#include "../algebra/Vector.h"

using namespace std;
using namespace algebra;

class Light {
private:
    Vector position;
    Vector ambient;
    Vector diffuse;
    Vector specular;
    float radius;
    static mt19937 rnd_gen;
    static uniform_real_distribution<float> distribution;

public:
    Light(Vector position, Vector ambient, Vector diffuse, Vector specular, float radius);
    Vector RandomPoint();
    Vector Position();
    Vector Ambient();
    Vector Diffuse();
    Vector Specular();
    float Radius();
};

#endif
