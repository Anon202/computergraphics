#include <cmath>
#include "SphereTexture.h"

SphereTexture::SphereTexture(const char* image_name, float r, Vector c) :
    Texture(image_name), radius(r), center(c) {
}

MappedCoords SphereTexture::MapObjectCoords(Vector position) const {
    Vector d = (position - this->center).Normalized();
    float u = 0.5 + atan2(d.z, d.x)/(2 * M_PI);
    float v = 0.5 - asin(d.y)/M_PI;
    return MappedCoords{u, v};
}
