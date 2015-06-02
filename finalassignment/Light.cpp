#include "Light.h"

random_device rd;

mt19937 Light::rnd_gen(rd());
uniform_real_distribution<float> Light::distribution(-2.0, 2.0);

Light::Light(Vector position, Vector ambient, Vector diffuse, Vector specular, float radius) :
    position(position), ambient(ambient), diffuse(diffuse), specular(specular), radius(radius) {
}

Vector Light::RandomPoint() {
    Vector r = Vector(distribution(rnd_gen),
                      distribution(rnd_gen),
                      distribution(rnd_gen));
    return this->position + (r * this->radius);
}

Vector Light::Position() {
    return this->position;
}

Vector Light::Ambient() {
    return this->ambient;
}

Vector Light::Diffuse() {
    return this->diffuse;
}

Vector Light::Specular() {
    return this->specular;
}

float Light::Radius() {
    return this->radius;
}
