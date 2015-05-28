#include "Vector.h"
#include <cmath>
#include <iomanip>

namespace algebra {

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

Vector::Vector() : x(0), y(0), z(0) {}

Vector Vector::Add(const Vector& v) const {
    return Vector(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector Vector::Subtract(const Vector& v) const {
    return Vector(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector Vector::Cross(const Vector& v) const {
    float newx = this->y*v.z - this->z*v.y;
    float newy = this->z*v.x - this->x*v.z;
    float newz = this->x*v.y - this->y*v.x;
    return Vector(newx, newy, newz);
}

float Vector::Dot(const Vector& v) const {
    return this->x*v.x + this->y*v.y + this->z*v.z;
}

float Vector::Length() const {
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z); 
}

Vector& Vector::Normalize() {
    float invLength = 1.0 / this->Length();
    this->x *= invLength;
    this->y *= invLength;
    this->z *= invLength;
    return *this;
}

Vector Vector::Normalized() const {
    Vector v(*this);
    return v.Normalize();
}

Vector Vector::ScalarMult(float c) const {
    return Vector(this->x*c, this->y*c, this->z*c);
}

Vector Vector::MultCoordwise(const Vector& v) const {
    return Vector(this->x * v.x, this->y * v.y, this->z * v.z);
}

void Vector::Print(const char *name) const {
    cout << name << ": " << *this << endl;
}

Vector Vector::operator-() const {
    return this->ScalarMult(-1);
}

Vector Vector::operator-(const Vector& rhs) const {
    return this->Subtract(rhs);
}

Vector Vector::operator+(const Vector& rhs) const {
    return this->Add(rhs);
}

Vector& Vector::operator +=(const Vector& v) {
   if (&v == this) {
        return *this;
   }
   this->x += v.x;
   this->y += v.y;
   this->z += v.z;
   return *this;
}

Vector Vector::operator*(float f) const {
    return this->ScalarMult(f);
}

ostream& operator<<(ostream &o, const Vector& v) {
    o << setprecision(5) << v.x << " " << v.y << " " << v.z;
    return o;
}

}
