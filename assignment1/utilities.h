#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <cmath>

float radToDeg(float rad) {
    return rad * 180.0/M_PI;
}

float degToRad(float deg) {
    return deg * M_PI/180.0;
}

#endif
