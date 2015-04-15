#include "Vector.h"
#include "Matrix.h"

using namespace algebra;

class Camera {
    private:
        Vector position;
        Vector rotation;
        Vector viewdir;
        Vector up;
        Vector xdir;

    public:
        const double nearPlane, farPlane, fov;
        Camera(double near, double far, double fov, Vector pos, Vector rot);
        Vector Position();
        Vector Rotation();
        Vector ViewDirection();
        Vector Up();
        void Move(char dir);
        void Rotate(char dir);
        Matrix lookAt();
};
