#include "Camera.h"

Camera::Camera() {
    this->position = Vec3f(0,0,0);
    this->up = Vec3f(0,1,0);
    this->lookAt = Vec3f(0,0,0);
}

Camera::Camera(Vec3f pos, Vec3f u, Vec3f look) : position(pos), up(u), lookAt(look) {
    CalculateWUV();
}

void Camera::CalculateWUV() {
    w = (lookAt - position).normalize();
    u = up.cross(w).normalize();
    v = w.cross(u);
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
