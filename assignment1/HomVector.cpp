#include "HomVector.h"

namespace algebra {
    HomVector::HomVector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

    Vector HomVector::homogenize() {
        if (this->w == 0.0) {
            // TODO: throw exception
            cerr << "Homogenize: w = 0" << endl;
            return Vector(9999999, 9999999, 9999999);
        }
        return Vector(this->x/this->w, this->y/this->w, this->z/this->w);
    }

    void HomVector::print(char* name) {
        cout << name << ": " << this; 
    }

    ostream& operator<<(ostream &o, const HomVector& v) {
        o << v.x << " " << v.y << " " << v.z << " " << v.w;
        return o;
    }
}
