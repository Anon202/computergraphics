#include "Image.h"
#include <bmpfile.h>
#include <cstdlib>
#include <iostream>

using namespace std;

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

void Image::Save(void) {
    bmpfile_t *bmp;
    if ((bmp = bmp_create(this->GetWidth(), this->GetHeight(), 8)) == NULL) {
        cerr << "Invalid depth value: " << 1 << endl;
        exit(1);
    }
    for (int i = 0; i < this->GetWidth(); i++) {
        for (int j = 0; j < this->GetHeight(); j++) {
            Vec3f p = this->GetPixel(i, j);
            rgb_pixel_t pixel = {(uint8_t)(p.b*255), (uint8_t)(p.g*255), (uint8_t)(p.r*255), 0};
            bmp_set_pixel(bmp, i, this->GetHeight() - j, pixel);
        }
    }
    bmp_save(bmp, "image.bmp");
    bmp_destroy(bmp);
}
