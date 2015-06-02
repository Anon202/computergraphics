#include "Ray.h"

random_device rdev;

float Ray::rayEps = 0.001f;
float Ray::tMax = 1e20f;
mt19937 Ray::rnd_gen(rdev());
uniform_real_distribution<float> Ray::distribution(0.0, 1.0);

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

void Ray::RandomlyPerturbDirection(float degree) {
    // Construct orthornormal basis (u, v)
    Vector w = this->d.Normalized();
    Vector t = w;
    if (t.x < t.y && t.x < t.z) {
        t.x = 1.0;
    } else if (t.y < t.x && t.y < t.z) {
        t.y = 1.0;
    } else {
        t.z = 1.0;
    }
    Vector u = t.Cross(w).Normalized();
    Vector v = w.Cross(u);
    
    // Perturb direction
    float ud = -degree/2.0 + distribution(rnd_gen)*degree;
    float vd = -degree/2.0 + distribution(rnd_gen)*degree;
    this->d = (w + u*ud + v*vd).Normalized();
}
