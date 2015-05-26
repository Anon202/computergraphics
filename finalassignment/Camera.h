#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec3.h"

class Camera {
public:
    Vec3f position;
    Vec3f up;
    Vec3f lookAt;
    Vec3f w, u, v;
    Camera();
    Camera(Vec3f pos, Vec3f u, Vec3f look);
    void CalculateWUV();
    void Move(char dir);
};

#endif
