#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "../EasyBMP/EasyBMP.h"
#include "../../algebra/Vector.h"

using namespace algebra;

typedef struct _MappedCoords {
    float u, v;
} MappedCoords;

enum class TextureConf {
    TEXTURE_AND_COLOR,
    ONLY_COLOR,
    ONLY_TEXTURE
};

class Texture {
protected:
    BMP bmp;
public:
    explicit Texture(const char* image_name);
    Color GetColor(float u, float v);
    virtual MappedCoords MapObjectCoords(Vector position) const = 0;
};

#endif
