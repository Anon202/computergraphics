#include "../algebra/Vector.h"
#include "../algebra/Matrix.h"

using namespace algebra;

class Camera {
    private:
        Vector position;
        Vector viewdir;
        Vector up;
        Vector xdir;

    public:
        const double near_plane, far_plane, fov;
        Camera(double near, double far, double fov, Vector pos);
        Vector Position();
        Vector ViewDirection();
        Vector Up();
        void Move(char dir);
        void Rotate(char dir);
        Matrix LookAt();
};
