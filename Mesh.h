#include <vector>
#include <string>
#include "Vector.h"
#include "Matrix.h"

using namespace algebra;
using namespace std;

typedef struct _Triangle {
	int vInds[3]; //vertex indices
} Triangle;

typedef struct _BoundingSphere {
    float radius;
    Vector center;
    Vector original_center;
} BoundingSphere;

class Mesh {
    private:
        static int bounding_type;
        int nv, nt;
        bool is_bounding = false;
        Vector rotation;
        Vector translation;
        Vector scale;
        BoundingSphere bounding_sphere;
        Mesh(vector<Vector> vertices, vector<Vector> vnorms, vector<Triangle> triangles, bool is_bounding);
        Mesh(int nv, int nt, float *vArr, int *tArr, bool is_bounding);
        void ComputeBoundingSphere();
        void UpdateBoundingVolume();

    public:
        static const int SPHERE_BOUNDING = 0;
        vector<Vector> vertices;
        vector<Vector> vnorms;
        vector<Triangle> triangles;
        unsigned int vbo, ibo, vao; // OpenGL handles for rendering
        Mesh *bounding_volume;
        static Mesh Load(string model_name, bool is_bounding);
        static void BoundingType(int bounding_type);
        static int BoundingType();
        int NumVertices();
        int NumTriangles();
        bool IsBounding();
        Matrix TransformationMatrix();
        void Rotate(char dir);
        void Move(char dir);
        void Scale(char dir);
        void Rotate(char dir, float units);
        void Move(char dir, float units);
        void Scale(char dir, float units); 
        void Rotate(Vector v);
        void Move(Vector v);
        void Scale(Vector v);
        void UniformScale(bool enlarge);
        float BoundingSphereRadius();
        Vector BoundingSphereCenter();
};
