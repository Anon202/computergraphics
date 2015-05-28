#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../algebra/Vector.h"

using namespace algebra;

class Camera {
public:
    Vector position;
    Vector up;
    Vector lookAt;
    Vector w, u, v;
    Camera();
    Camera(Vector pos, Vector u, Vector look);
    void CalculateWUV();
    void Move(char dir);
};

#endif
