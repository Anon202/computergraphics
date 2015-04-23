#ifndef _HOMVECTOR_H_
#define _HOMVECTOR_H_

#include "Vector.h"
#include <iostream>

using namespace std;

namespace algebra {

class HomVector {
    public:
        const float x, y, z, w; 
        HomVector(float x, float y, float z, float w);
        Vector Homogenize();
        void Print(char *name);
        friend ostream& operator<<(ostream &o, const HomVector& v);
};

}

#endif
