#include "Image.h"

Image::Image(const int w, const int h) : width(w), height(h) {
    this->pixels = new Vec3f[this->width * this->height];
}

Image::~Image(void) {
    delete [] this->pixels;
}

int Image::GetWidth(void) const {
    return this->width;
}

int Image::GetHeight(void) const {
    return this->height;
}

void Image::SetPixel(const int x, const int y, const Vec3f &c) {
    this->pixels[y * this->width + x] = c;
}

Vec3f Image::GetPixel(const int x, const int y) {
    return this->pixels[y * this->width + x];
}

Vec3f *Image::GetPixelBufferPtr(void) {
    return this->pixels;
}
