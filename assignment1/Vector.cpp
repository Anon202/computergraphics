#include "Vector.h"
#include <cmath>
#include <iomanip>

namespace algebra {

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

Vector::Vector() : x(0), y(0), z(0) {}

Vector Vector::Add(Vector v) {
    return Vector(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector Vector::Subtract(Vector v) {
    return Vector(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector Vector::Cross(Vector v) {
    float newx = this->y*v.z - this->z*v.y;
    float newy = this->z*v.x - this->x*v.z;
    float newz = this->x*v.y - this->y*v.x;
    return Vector(newx, newy, newz);
}

float Vector::Dot(Vector v) {
    return this->x*v.x + this->y*v.y + this->z*v.z;
}

float Vector::Length() {
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z); 
}

Vector Vector::Normalized() {
    float length = this->Length();
    return Vector(this->x/length, this->y/length, this->z/length);
}

Vector Vector::ScalarMult(float c) {
    return Vector(this->x*c, this->y*c, this->z*c);
}

void Vector::Print(const char *name) {
    cout << name << ": " << *this << endl;
}

Vector Vector::operator-() {
    this->x = -this->x;
    this->y = -this->y;
    this->z = -this->z;
    return *this;
}

Vector Vector::operator-(const Vector& rhs) {
    return this->Subtract(rhs);
}

Vector Vector::operator+(const Vector& rhs) {
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

ostream& operator<<(ostream &o, const Vector& v) {
    o << setprecision(5) << v.x << " " << v.y << " " << v.z;
    return o;
}

}
