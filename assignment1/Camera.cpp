#include <cmath>
#include "Camera.h"

Camera::Camera(double near, double far, double fov, Vector pos, Vector rot) :
    nearPlane(near), farPlane(far), fov(fov) {
    this->position = pos;
    this->rotation = rot;
    this->viewdir = Vector(0, 0, -1);
    this->up = Vector(0, 1, 0);
    this->xdir = this->viewdir.cross(this->up);
}

Vector Camera::Position() {
    return this->position;
}

Vector Camera::Rotation() {
    return this->rotation;
}

Vector Camera::ViewDirection() {
    return this->viewdir;
}

Vector Camera::Up() {
    return this->up;
}

void Camera::Move(char dir) {
    int f = 1;
    if (dir < 'A' || dir > 'Z') {
        f = -1;
    }
    switch (dir) {
        case 'x': case 'X':
            //this->position += this->xdir.scalarMult(f);
            this->position.x += f*0.2;
            break;
        case 'y': case 'Y':
            //this->position += this->up.scalarMult(f);
            this->position.y += f*0.2;
            break;
        case 'z': case 'Z':
            //this->position += this->viewdir.scalarMult(f);
            this->position.z += f*0.2;
            break;
        default:
            // TODO: throw exception
            break;
    }
}

void Camera::Rotate(char dir) {
    int f = 1;
    if (dir < 'A' || dir > 'Z') {
        f = -1;
    }
    switch (dir) {
        case 'i': case 'I':
            this->rotation.x += f*M_PI/100;
            break;
        case 'j': case 'J':
            this->rotation.y += f*M_PI/100;
            break;
        case 'k': case 'K':
            this->rotation.z += f*M_PI/100;
            break;
        default:
            // TODO: throw exception
            break;
    }
}

Matrix Camera::lookAt() {
    Vector w = -(this->viewdir.normalized());
    Vector upw = this->up.cross(w);
    Vector u = upw.scalarMult(1.0/upw.length());
    Vector v = w.cross(u);
    Matrix m = Matrix();
    m.e[0] = u.x; m.e[4] = u.y; m.e[ 8] = u.z; m.e[12] = 0.0;
    m.e[1] = v.x; m.e[5] = v.y; m.e[ 9] = v.z; m.e[13] = 0.0;
    m.e[2] = w.x; m.e[6] = w.y; m.e[10] = w.z; m.e[14] = 0.0;
    m.e[3] = 0.0; m.e[7] = 0.0; m.e[11] = 0.0; m.e[15] = 1.0;
    return m * Matrix::translation(-this->position);
}
