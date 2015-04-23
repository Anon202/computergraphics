#include <vector>
#include <string>
#include "Vector.h"
#include "Matrix.h"

using namespace algebra;
using namespace std;

typedef struct _Triangle {
	int vInds[3]; //vertex indices
} Triangle;

class Mesh {
    private:
        int nv, nt;
        Vector rotation;
        Vector translation;
        Vector scale;
        Mesh(int nv, int nt);

    public:
        vector<Vector> vertices;
        vector<Vector> vnorms;
        vector<Triangle> triangles;
        unsigned int vbo, ibo, vao; // OpenGL handles for rendering
        Mesh(int nv, int nt, float *vArr, int *tArr);
        static Mesh Load(string model_name);
        int NumVertices();
        int NumTriangles();
        void SetScale(Vector scale);
        void SetRotation(Vector rotation);
        void SetTranslation(Vector scale);
        Matrix TransformationMatrix();
        void Rotate(char dir);
        void Move(char dir);
        void Scale(char dir);
};
