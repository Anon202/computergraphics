//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <glew.h>
#include <freeglut.h>
#include <vector>

#include "Matrix.h"
#include "Vector.h"
#include "shaders.h"
#include "Mesh.h"
#include "Camera.h"

using namespace std;
using namespace algebra;

int screen_width = 1024;
int screen_height = 768;
int selected_obj = 0;
bool moving_cam = true;
bool use_parallel_proj = false;

vector<Mesh*> meshList;  // Pointer to linked list of triangle meshes
Camera cam = Camera(1, 10000, 60, Vector(0, 0, 20), Vector(0,0,0)); // Setup the camera parameters
GLuint shprg; // Shader program id

// Transform matrices
// V is view transform
// P is the projection transform
// PV = P * V
Matrix V, P, PV;


void prepareShaderProgram() {
	shprg = glCreateProgram();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_src, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_src, NULL);
	glCompileShader(vs);

	glAttachShader(shprg, vs);
	glAttachShader(shprg, fs);

	glLinkProgram(shprg);
}


void prepareMesh(Mesh* mesh) {
	int sizeVerts = mesh->nvertices() * 3 * sizeof(float);
	int sizeCols = mesh->nvertices() * 3 * sizeof(float);
	int sizeTris = mesh->ntriangles() * 3 * sizeof(int);

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
    Matrix PVW = PV * mesh->transformationMatrix();

	// Pass the viewing transform to the shader
    GLint loc_PV = glGetUniformLocation(shprg, "PV");
	glUniformMatrix4fv(loc_PV, 1, GL_FALSE, PVW.e);

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
            (void *)(mesh->nvertices() * 3 *sizeof(float)));
	
	// To accomplish wireframe rendering (can be removed to get filled triangles)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    // Enable Z-buffer
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

	// Draw all triangles
	glDrawElements(GL_TRIANGLES, mesh->ntriangles() * 3, GL_UNSIGNED_INT, NULL); 
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		
	// Assignment1: Calculate the transform to view coordinates yourself 
	// Replace this hard-coded transform. 
	// M should be calculated from camera parameters
	Vector c = {.x = cam.Position().x, .y = cam.Position().y,
                .z = -cam.Position().z }; 
    V = Matrix::rotation('z', cam.Rotation().z)
            * Matrix::rotation('y', cam.Rotation().y)
            * Matrix::rotation('x', cam.Rotation().x)
            * Matrix::translation(c);
    // Assignment1: Calculate the projection transform yourself 
	// Replace this hard-coded transform. 	
	// P should be calculated from camera parameters
    float aspect = (float)screen_width / screen_height;
    P = Matrix::perspectiveProj(cam.nearPlane, cam.farPlane, cam.fov, aspect);

    if (use_parallel_proj) {
        P = Matrix::parallelProj(screen_height/40.0,
                screen_width/40.0, -screen_height/40.0,
                -screen_width/40.0, cam.nearPlane, cam.farPlane);
    }

    //Matrix Mcam = cam.lookAt();
    PV = P * V;

	glUseProgram(shprg);
   
    for (unsigned int i = 0; i < meshList.size(); i++) {
        renderMesh(meshList[i]);
    }

	glFlush();
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
    case 'p': case 'P':
        use_parallel_proj = true;
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
            char dir = key;
            if (dir >= 'A' && dir <= 'Z') dir = key - 'A' + 'a';
            dir = (dir == 'w')? 'x' : ((dir == 'e')? 'y' : 'z');
            if (key >= 'A' && key <= 'Z') dir = dir - 'a' + 'A';
            cout << dir << endl;
            meshList[selected_obj]->Scale(dir);
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
    Mesh tr = Mesh::load("models/triceratops.obj");
    tr.setScale(Vector(2, 2, 2));
	meshList.push_back(&tr);

    init();
	glutMainLoop();

	cleanUp();	
	return 0;
}
