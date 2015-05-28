#include "Camera.h"

Camera::Camera() {
    this->position = Vector(0,0,0);
    this->up = Vector(0,1,0);
    this->lookAt = Vector(0,0,0);
}

Camera::Camera(Vector pos, Vector u, Vector look) : position(pos), up(u), lookAt(look) {
    CalculateWUV();
}

void Camera::CalculateWUV() {
    w = (lookAt - position).Normalized();
    u = up.Cross(w).Normalized();
    v = w.Cross(u);
}

void Camera::Move(char dir) {
    float k = (dir >= 'a' && dir <= 'z')? -0.1 : 0.1;
    switch(dir) {
        case 'x': case 'X':
            this->position.x += k;
            break;
        case 'y': case 'Y':
            this->position.y += k;
            break;
        case 'z': case 'Z':
            this->position.z += k;
            break;
    }
    this->CalculateWUV();
}
