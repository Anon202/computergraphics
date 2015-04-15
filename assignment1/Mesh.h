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
        
        Mesh(int nov, int nof);

    public:
        vector<Vector> vertices;
        vector<Vector> vnorms;
        vector<Triangle> triangles;
        unsigned int vbo, ibo, vao; // OpenGL handles for rendering
        Mesh(int nv, int nt, float *vArr, int *tArr);
        static Mesh load(string model_name);
        int nvertices();
        int ntriangles();
        void setScale(Vector scale);
        void setRotation(Vector rotation);
        void setTranslation(Vector scale);
        Matrix transformationMatrix();
};
