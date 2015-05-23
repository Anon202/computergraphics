#include "Ray.h" 

float Ray::rayEps = 0.001f;
float Ray::tMax = 1e20f;

Ray::Ray() : tClip(tMax) {
}

Ray::Ray(const Vec3f& o, const Vec3f& d, float tclip) : o(o), d(d), tClip(tclip) {
}
	
void Ray::EpsMoveStartAlongSurfaceNormal(const Vec3f& n) {
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
