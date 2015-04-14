#include "Vector.h"
#include <cmath>
#include <iomanip>

namespace algebra {

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

Vector::Vector() : x(0), y(0), z(0) {}

Vector Vector::add(Vector v) {
    return Vector(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector Vector::subtract(Vector v) {
    return Vector(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector Vector::cross(Vector v) {
    float newx = this->y*v.z - this->z*v.y;
    float newy = this->z*v.x - this->x*v.z;
    float newz = this->x*v.y - this->y*v.x;
    return Vector(newx, newy, newz);
}

float Vector::dot(Vector v) {
    return this->x*v.x + this->y*v.y + this->z*v.z;
}

float Vector::length() {
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z); 
}

Vector Vector::normalized() {
    float length = this->length();
    return Vector(this->x/length, this->y/length, this->z/length);
}

Vector Vector::scalarMult(float c) {
    return Vector(this->x*c, this->y*c, this->z*c);
}

void Vector::print(const char *name) {
    cout << name << ": " << *this << endl;
}

Vector Vector::operator-(const Vector& rhs) {
    return this->subtract(rhs);
}

Vector Vector::operator+(const Vector& rhs) {
    return this->add(rhs);
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
