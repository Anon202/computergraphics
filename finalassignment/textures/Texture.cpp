#include <iostream>
#include "Texture.h"

using namespace std;

Texture::Texture(const char* bmpfilename) {
    bool r = this->bmp.ReadFromFile(bmpfilename);
    if (!r) {
        cerr << "Couldn't open the provived bmp file" << endl;
        exit(-1);
    }
}

Color Texture::GetColor(float u, float v) {
    u = 1.0 - u;
    v = 1.0 - v;
    if (u < 0) u = 1.0 + u;
    if (v < 0) v = 1.0 + v;
    if (u >= 1) u = u - 1.0;
    if (v >= 1) v = v - 1.0;
    int x = (int)(u * (this->bmp.TellWidth() - 1));
    int y = (int)(v * (this->bmp.TellHeight() - 1));
    return Vector(bmp(x, this->bmp.TellHeight() - y - 1)->Red / 255.0,
                  bmp(x, this->bmp.TellHeight() - y - 1)->Green / 255.0,
                  bmp(x, this->bmp.TellHeight() - y - 1)->Blue / 255.0);
}
