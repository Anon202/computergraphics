#include "Ray.h"

random_device rdev;

float Ray::rayEps = 0.001f;
float Ray::tMax = 1e20f;
mt19937 Ray::rnd_gen(rdev());
uniform_real_distribution<float> Ray::distribution(-1.0, 1.0);

Ray::Ray() : tClip(tMax) {
}

Ray::Ray(const Vector& o, const Vector& d, float tclip) : o(o), d(d), tClip(tclip) {
}
	
void Ray::EpsMoveStartAlongSurfaceNormal(const Vector& n) {
    this->o = this->o + n * rayEps;
}
void Ray::EpsMoveStartAlongDir() {
    this->o = this->o + d * rayEps;
}
	
void Ray::IncRayEps() {
    rayEps *= 10.0f;
}

void Ray::DecRayEps() {
    rayEps /= 10.0f;
}

void Ray::RandomlyMoveDirection() {
    Vector r = Vector(distribution(rnd_gen),
                      distribution(rnd_gen),
                      distribution(rnd_gen));
    this->d = (this->d + r).Normalized();

}
