#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "../algebra/Vector.h"

using namespace algebra;

class Image {
private:
    int width, height;
    Vector *pixels;

public:
    Image(const int w, const int h);
    ~Image(void);
    int GetWidth(void) const;
    int GetHeight(void) const;
    void SetPixel(const int x, const int y, const Vector &c);
    Vector GetPixel(const int x, const int y);
    Vector *GetPixelBufferPtr(void);
    void Save(void);
};

#endif
