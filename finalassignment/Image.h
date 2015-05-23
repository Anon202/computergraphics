#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Vec3.h"

class Image {
private:
    int width, height;
    Vec3f *pixels;

public:
    Image(const int w, const int h);
    ~Image(void);
    int GetWidth(void) const;
    int GetHeight(void) const;
    void SetPixel(const int x, const int y, const Vec3f &c);
    Vec3f GetPixel(const int x, const int y);
    Vec3f *GetPixelBufferPtr(void);
};

#endif
