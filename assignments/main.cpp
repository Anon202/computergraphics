//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <glew.h>
#include <freeglut.h>
#include <vector>
#include <ctime>
#include <cstring>
#include <fstream>

#include "../algebra/Matrix.h"
#include "../algebra/Vector.h"
#include "../algebra/HomVector.h"
#include "../common/Camera.h"
#include "Mesh.h"

using namespace std;
using namespace algebra;

int screen_width = 1024;
int screen_height = 768;
int selected_obj = 0;
bool moving_cam = true;
bool use_parallel_proj = false;
bool frustum_culling = false;
string shaders[4] = {
    "cartoon",
    "gouraud",
    "phong",
    "example"
};
int curr_shader = 0;

vector<Mesh*> meshList;  // Pointer to linked list of triangle meshes
Camera cam = Camera(1, 10000, 60, Vector(0, 0, 10)); // Setup the camera parameters
GLuint shprg; // Shader program id

// Transform matrices
// V is view transform
// P is the projection transform
// PV = P * V
Matrix V, P, PV;
Vector planes[6] = {
    Vector(1,0,0),
    Vector(-1,0,0),
    Vector(0,1,0),
    Vector(0,-1,0),
    Vector(0,0,1),
    Vector(0,0,-1)
};

string readShaderFile(const char *file_path) {
    string content;
    ifstream fs(file_path, std::ios::in);
    
    if (!fs.is_open()) {
        cerr << "Couldn't open " << file_path << "." << endl;
        return "";
    }

    string line = "";
    while (!fs.eof()) {
        getline(fs, line);
        content.append(line + "\n");
    }
    fs.close();
    return content;
}

void checkSuccessfulCompilation(GLuint shader) {
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        cout << "Error compiling shader:" << endl;
        for (unsigned int i = 0; i < errorLog.size(); i++) {
            cout << errorLog[i];
        }
        cout << endl;
        //glDeleteShader(shader); // Don't leak the shader.
        return;
    }
}

void prepareShaderProgram() {
	shprg = glCreateProgram();
	const char* fsfile = ("shaders/" + shaders[curr_shader] + "fs.glsl").c_str();
    string fs_str = readShaderFile(fsfile);
    const char* fs_src = fs_str.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
	glCompileShader(fs);
    checkSuccessfulCompilation(fs);

	const char* vsfile = ("shaders/" + shaders[curr_shader] + "vs.glsl").c_str();
    string vs_str = readShaderFile(vsfile);
    const char* vs_src = vs_str.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_src, NULL);
	glCompileShader(vs);
    checkSuccessfulCompilation(vs);

	glAttachShader(shprg, vs);
	glAttachShader(shprg, fs);

	glLinkProgram(shprg);
}


void prepareMesh(Mesh* mesh) {
	int sizeVerts = mesh->NumVertices() * 3 * sizeof(float);
	int sizeCols = mesh->NumVertices() * 3 * sizeof(float);
	int sizeTris = mesh->NumTriangles() * 3 * sizeof(int);

	// Allocate GPU buffer and load mesh data
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts + sizeCols, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerts, (void *)&mesh->vertices.at(0));
	glBufferSubData(GL_ARRAY_BUFFER, sizeVerts, sizeCols, (void *)&mesh->vnorms.at(0));

	// For specification of the data stored in the vbo
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	// Allocate GPU index buffer and load mesh indices
	glGenBuffers(1, &mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeTris, (void *)&mesh->triangles.at(0), GL_STATIC_DRAW);
}

void renderMesh(Mesh* mesh) {
	
	// Assignment 1: Apply the transforms from local mesh coordinates to world coordinates here
	// Combine it with the viewing transform that is pass to the shader below
    Matrix PVM = PV * mesh->TransformationMatrix();

	// Pass the viewing transform to the shader
    GLint loc_PVM = glGetUniformLocation(shprg, "PVM");
	glUniformMatrix4fv(loc_PVM, 1, GL_FALSE, PVM.e);

    // Pass the material properties to the shader
    Material material = mesh->MaterialProperties();

    GLint loc_mAmbient = glGetUniformLocation(shprg, "mAmbient");
    glUniform3f(loc_mAmbient, material.ambient.x, material.ambient.y, material.ambient.z);
    
    GLint loc_mDiffuse = glGetUniformLocation(shprg, "mDiffuse");
    glUniform3f(loc_mDiffuse, material.diffuse.x, material.diffuse.y, material.diffuse.z);
    
    GLint loc_mSpecular = glGetUniformLocation(shprg, "mSpecular");
    glUniform3f(loc_mSpecular, material.specular.x, material.specular.y, material.specular.z);
    
    GLint loc_mShininess = glGetUniformLocation(shprg, "mShininess");
    glUniform1f(loc_mShininess, material.shininess);
    

	// Select current resources 
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBindBuffer(GL_VERTEX_ARRAY, mesh->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

	// Set up vertex array
	GLint vPos = glGetAttribLocation(shprg, "vPos");
	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Set up normal array
	GLint vNorm = glGetAttribLocation(shprg, "vNorm");
	glEnableVertexAttribArray(vNorm);
	glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, 0,
            (void *)(mesh->NumVertices() * 3 *sizeof(float)));
	
    if (mesh->IsBounding()) {
	    // To accomplish wireframe rendering
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    }
    // Enable Z-buffer
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

	// Draw all triangles
	glDrawElements(GL_TRIANGLES, mesh->NumTriangles() * 3, GL_UNSIGNED_INT, NULL); 
}

void display(void) {
    //clock_t start = clock();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		
	// Assignment1: Calculate the transform to view coordinates yourself 
	// Replace this hard-coded transform. 
	// M should be calculated from camera parameters
    Matrix V = cam.LookAt();
    
    // Assignment1: Calculate the projection transform yourself 
	// Replace this hard-coded transform. 	
	// P should be calculated from camera parameters
    float aspect = (float)screen_width / screen_height;
    P = Matrix::PerspectiveProj(cam.near_plane, cam.far_plane, cam.fov, aspect);

    if (use_parallel_proj) {
        P = Matrix::ParallelProj(screen_height/40.0,
                screen_width/40.0, -screen_height/40.0,
                -screen_width/40.0, cam.near_plane, cam.far_plane);
    }

    PV = P * V;

	glUseProgram(shprg);
    if (frustum_culling) {
        //bool nothing_to_render = true;
        for (unsigned int i = 0; i < meshList.size(); i++) {
            Matrix PVW = PV * meshList[i]->TransformationMatrix();
            Vector center = PVW * meshList[i]->BoundingSphereCenter();
            bool draw = true;
            for (int j = 0; j < 6; j++) {
                HomVector v = PVW.Transposed().MultiplyH(planes[j]);
                Vector normal = Vector(v.x, v.y, v.z).Normalized();
                float distance = v.w / Vector(v.x, v.y, v.z).Length();
                if (normal.Dot(center) + distance <= -meshList[i]->BoundingSphereRadius()) {
                    draw = false;
                    break;
                }
            }
            if (draw) {
                //cout << "Rendering object #" << i << endl;
                renderMesh(meshList[i]);
                renderMesh(meshList[i]->bounding_volume);
                //nothing_to_render = false;
            }
        }

        //if (nothing_to_render) {
        //    cout << "Nothing to render" << endl;
        //}
    } else {
        for (unsigned int i = 0; i < meshList.size(); i++) {
            renderMesh(meshList[i]);
        }
    }

	glFlush();
    
    //cout << "Rendering time: " << (std::clock() - start)/(double)(CLOCKS_PER_SEC/1000) << " ms" << endl;
}

void changeSize(int w, int h) {
	screen_width = w;
	screen_height = h;
	glViewport(0, 0, screen_width, screen_height);
}

void keypress(unsigned char key, int x, int y) {
    if (key >= '0' && key <= '9') {
        if ((unsigned int)(key - '0') > meshList.size()) {
            return;
        }
        selected_obj = key - '0';
        moving_cam = false;
        return;
    }
    switch(key) {
    case 'm':  case 'M':
        curr_shader = (curr_shader + 1)%(sizeof(shaders)/sizeof(shaders[0]));
	    prepareShaderProgram(); 
        break;
    case 'o': case 'O':
        frustum_culling = !frustum_culling;
        break;
    case 'p': case 'P':
        use_parallel_proj = !use_parallel_proj;
        break;
    case 'c': case 'C':
        moving_cam = true;
        break;
	  case 'z': case 'Z':
  	case 'y': case 'Y':
    case 'x': case 'X':
        if (moving_cam) cam.Move(key);
        else meshList[selected_obj]->Move(key);
        break;
    case 'i': case 'I':
    case 'j': case 'J':
    case 'k': case 'K':
        if (moving_cam) cam.Rotate(key);
        else meshList[selected_obj]->Rotate(key);
        break;
    case 'w': case 'W':
    case 'e': case 'E':
    case 'r': case 'R':
        if (!moving_cam) {
            meshList[selected_obj]->UniformScale(key >= 'A' && key <= 'Z');
        }
        break;
	case 'Q':
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void init(void) {
	// Setup OpenGL buffers for rendering of the meshes
    for (unsigned int i = 0; i < meshList.size(); i++) {
        prepareMesh(meshList[i]);
        prepareMesh(meshList[i]->bounding_volume);
    }
	
	// Compile and link shader program
	prepareShaderProgram(); 
}

void cleanUp(void) {	
	// Free openGL resources
	// ...

	// Free meshes
	// ...
}

int main(int argc, char **argv) {	
	// Setup freeGLUT	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("DVA304 Assignments");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keypress);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Specify your preferred OpenGL version and profile
	glutInitContextVersion(4, 5);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);	
	glutInitContextProfile(GLUT_CORE_PROFILE);

	// Use an OpenGL Loading Library to get access to modern core features as well as extensions
	GLenum err = glewInit(); 
	if (GLEW_OK != err) {
        cerr << "Error: " << glewGetErrorString(err) << endl;
        return 1;
    }

	// Output OpenGL version info
	cout << "GLEW version: " << glewGetString(GLEW_VERSION) << endl;
	cout << "OpenGL version: " << (const char *)glGetString(GL_VERSION) << endl;
	cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl << endl;

	// Insert the 3D models you want in your scene here in a vector of meshes
    Mesh::BoundingType(Mesh::SPHERE_BOUNDING);
    Mesh bunny = Mesh::Load("models/bunny.obj", false);
    bunny.Scale(Vector(10, 10, 10));
    bunny.Move(Vector(5, -5, 10));
    
    Mesh tr = Mesh::Load("models/triceratops.obj", false);
    tr.Scale(Vector(0.3, 0.3, 0.3));
    tr.Move(Vector(-10, 15, -10));
	
    Mesh knot = Mesh::Load("models/knot.obj", false);
    knot.Move(Vector(-5, -100, -400));

    Mesh cow = Mesh::Load("models/cow.obj", false);
    cow.Scale(Vector(3, 3, 3));
    cow.Move(Vector(20, 10, 10));

    Mesh tp = Mesh::Load("models/teapot.obj", false);
    tp.Move(Vector(30, -20, -15));
    tp.Rotate(Vector(50, 0, 100));
   
    Mesh frog = Mesh::Load("models/frog.obj", false);
    frog.Rotate(Vector(0, 25, 25));
    frog.Move(Vector(-80,-30,-100));

    Mesh cube = Mesh::Load("models/cube.obj", false);
    cube.Rotate(Vector(0, 25, 0));
    cube.Move(Vector(-10, -23, 0));

    Mesh sphere = Mesh::Load("models/sphere.obj", false);
    sphere.Move(Vector(10, -23, 0));

    meshList.push_back(&bunny);
	meshList.push_back(&tr);
    meshList.push_back(&knot);
    meshList.push_back(&cow);
    meshList.push_back(&tp);
    meshList.push_back(&frog);
    meshList.push_back(&cube);
    meshList.push_back(&sphere);

    vector<Mesh> benchmark_meshes;
    if (argc == 2 && strcmp("--benchmark", argv[1]) == 0) { 
        int num_objs = 10000;
        for (int i = 0; i < num_objs; i++) {
            benchmark_meshes.push_back(Mesh::Load("models/sphere.obj", false));
        }
        for (int i = -(num_objs/2); i < num_objs/2; i++) {
            benchmark_meshes[i+num_objs/2].Move(Vector(20*i+20, 0, 0));
            meshList.push_back(&benchmark_meshes[i+num_objs/2]);
        }
    }

    init();
	glutMainLoop();

	cleanUp();	
	return 0;
}
