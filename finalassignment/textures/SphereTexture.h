#ifndef _SPHERE_TEXTURE_H_
#define _SPHERE_TEXTURE_H_

#include "Texture.h"
#include "../../algebra/Vector.h"

class SphereTexture : public Texture {
private:
    float radius;
    Vector center;
public:
    SphereTexture(const char* image_name, float r, Vector c);
    MappedCoords MapObjectCoords(Vector position) const;
};

#endif
