#include "Image.h"
#include "EasyBMP/EasyBMP.h"
#include <cstdlib>
#include <iostream>

using namespace std;

Image::Image(const int w, const int h) : width(w), height(h) {
    this->pixels = new Vector[this->width * this->height];
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

void Image::SetPixel(const int x, const int y, const Vector &c) {
    this->pixels[y * this->width + x] = c;
}

Vector Image::GetPixel(const int x, const int y) {
    return this->pixels[y * this->width + x];
}

Vector *Image::GetPixelBufferPtr(void) {
    return this->pixels;
}

unsigned int clamp(unsigned int x) {
    if (x > 255) {
        return 255;
    } else if (x < 0) {
        return 0;
    }
    return x;
}

void Image::Save(void) {
    BMP bmp;
    bmp.SetSize(this->GetWidth(), this->GetHeight());
    bmp.SetBitDepth(32);
    for (int i = 0; i < this->GetWidth(); i++) {
        for (int j = 0; j < this->GetHeight(); j++) {
            Vector p = this->GetPixel(i, j);
            int jcoord = this->GetHeight() - j - 1;
            bmp(i, jcoord)->Red = clamp((unsigned int)(p.r * 255));
            bmp(i, jcoord)->Green = clamp((unsigned int)(p.g * 255));
            bmp(i, jcoord)->Blue = clamp((unsigned int)(p.b * 255));
        }
    }
    bmp.WriteToFile("image.bmp");
}
