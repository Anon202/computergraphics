#include <cmath>
#include "Camera.h"
#include "../algebra/HomVector.h"

Camera::Camera(double near, double far, double fov, Vector pos) :
    near_plane(near), far_plane(far), fov(fov) {
    this->position = pos;    
    this->xdir = Vector(1, 0, 0);
    this->up = Vector(0, 1, 0);
    this->viewdir = this->xdir.Cross(this->up).Normalized();
}

Vector Camera::Position() {
    return this->position;
}

Vector Camera::ViewDirection() {
    return this->viewdir;
}

Vector Camera::Up() {
    return this->up;
}

void Camera::Move(char dir) {
    float f = 0.1;
    if (dir < 'A' || dir > 'Z') {
        f = -0.1;
    }
    switch (dir) {
        case 'x': case 'X':
            this->position += this->xdir.ScalarMult(f);
            break;
        case 'y': case 'Y':
            this->position += this->up.ScalarMult(f);
            break;
        case 'z': case 'Z':
            this->position += this->viewdir.ScalarMult(f);
            break;
        default:
            // TODO: throw exception
            break;
    }
}

void Camera::Rotate(char dir) {
    float angle = M_PI/100;
    if (dir < 'A' || dir > 'Z') {
        angle *= -1;
    }
    switch (dir) {
        case 'i': case 'I':
            this->up = Matrix::RotationAroundAxis(this->xdir, angle) * this->up;
            this->viewdir = Matrix::RotationAroundAxis(this->xdir, angle) * this->viewdir;
            break;
        case 'j': case 'J':
            this->viewdir = Matrix::RotationAroundAxis(this->up, angle) * this->viewdir;
            this->xdir = Matrix::RotationAroundAxis(this->up, angle) * this->xdir;
            break;
        case 'k': case 'K':
            this->xdir = Matrix::RotationAroundAxis(this->viewdir, angle) * this->xdir;
            this->up = Matrix::RotationAroundAxis(this->viewdir, angle) * this->up;
            break;
        default:
            // TODO: throw exception
            break;
    }
}

Matrix Camera::LookAt() {
    Matrix m = Matrix();
    m.e[0] = xdir.x;    m.e[4] = xdir.y;    m.e[ 8] = xdir.z;    m.e[12] = 0.0;
    m.e[1] = up.x;      m.e[5] = up.y;      m.e[ 9] = up.z;      m.e[13] = 0.0;
    m.e[2] = viewdir.x; m.e[6] = viewdir.y; m.e[10] = viewdir.z; m.e[14] = 0.0;
    m.e[3] = 0.0;       m.e[7] = 0.0;       m.e[11] = 0.0;       m.e[15] = 1.0;
    return m * Matrix::Translation(-this->position);
}
