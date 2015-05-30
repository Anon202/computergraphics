#include <iostream>
#include <cstdlib>
#include <glut.h>
#include <omp.h>

#include "Image.h"
#include "Sphere.h"
#include "Scene.h"
#include "SimpleRayTracer.h"
#include "../algebra/Vector.h"
#include "../common/Camera.h"

using namespace std;
using namespace algebra;

void glSetPixel(int x, int y, const Vector & c) {
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

SimpleRayTracer *rayTracer = NULL;
bool benchmark = false;
void shadowsCase(Scene* scene);
void testCase1(Scene* scene);
void reflectionsCase(Scene* scene);
void transparencyCase(Scene* scene);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    rayTracer->FireRays(&glSetPixel);

    glFlush();
}

void changeSize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glViewport(0,0,w,h);
}

void keypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'x': case 'X':
        case 'y': case 'Y':
        case 'z': case 'Z':
            rayTracer->cam.Move(key);
            display();
            break;
        case 'i': case 'I':
        case 'j': case 'J':
        case 'k': case 'K':
            rayTracer->cam.Rotate(key);
            display();
            break;
        case 's': case 'S':
            rayTracer->GetImage()->Save();
            break;
        case 'm': case 'M':
            benchmark = true;
            display();
            benchmark = false;
            break;
        case 'q': case 'Q':
            exit(0);
    }
}

void init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("SimpleRayTracer");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keypress);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    Scene* scene = new Scene;
    reflectionsCase(scene);

    Image *image = new Image(640, 480);

    rayTracer = new SimpleRayTracer(scene, image, Camera(Vector(0,0,10)));
}

void testCase1(Scene* scene) { 
    Material m1 = Material{
        .ambient = Vector(0, 0, 0.5),
        .diffuse = Vector(0.6, 0.4, 1.0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(-1.0f, 0.5f, -6.0f), 0.5f, m1));

    Material m2 = Material{
        .ambient = Vector(0.5, 0, 0),
        .diffuse = Vector(0.5, 0, 0),
        .specular = Vector(1, 1, 1),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(0.0f, 0.0f, -5.0f), 0.5f, m2));

    Material m3 = Material{
        .ambient = Vector(0, 0.5, 0),
        .diffuse = Vector(0, 0.5, 0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(0.5f, 0.0f, -4.0f), 0.5f, m3));
   
    Material m4 = Material{
        .ambient = Vector(0.8, 0.8, 0),
        .diffuse = Vector(0.3, 0.3, 0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 10,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(1.0f, 0.0f, -10.0f), 3.0f, m4));

    scene->Add(Light{
        .position = Vector(10, 5, -5),
        .ambient = Vector(0.4, 0.4, 0.4),
        .diffuse = Vector(0.8, 0.8, 0.8),
        .specular = Vector(1, 1, 1)
    });
}

void shadowsCase(Scene* scene) {
    Material m1 = Material{
        .ambient = Vector(0, 0, 0.5),
        .diffuse = Vector(0.6, 0.4, 1.0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(-1.0f, 0.0f, 2), 1, m1));

    Material m2 = Material{
        .ambient = Vector(0.5, 0, 0),
        .diffuse = Vector(0.5, 0, 0),
        .specular = Vector(1, 1, 1),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(1.5f, 0.0f, 2), 1, m2));
    
    scene->Add(Light{
        .position = Vector(-10,10,2),
        .ambient = Vector(0.4, 0.4, 0.4),
        .diffuse = Vector(0.8, 0.8, 0.8),
        .specular = Vector(1, 1, 1)
    });
}

void reflectionsCase(Scene* scene) {
    Material m1 = Material{
        .ambient = Vector(0, 0, 0.5),
        .diffuse = Vector(0.6, 0.4, 1.0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(-1.5f, 1.0f, 2.0f), 1, m1));

    Material m2 = Material{
        .ambient = Vector(0.5, 0, 0),
        .diffuse = Vector(0.5, 0, 0),
        .specular = Vector(1, 1, 1),
        .shininess = 5,
        .reflective = true,
        .transparency = true,
        .refractionIndex = 1.05
    };
    scene->Add(Sphere(Vector(1.2f, 0.0f, 5.0f), 1, m2));

    Material m3 = Material{
        .ambient = Vector(0, 0.5, 0),
        .diffuse = Vector(0, 0.5, 0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 1.5
    };
    scene->Add(Sphere(Vector(0.0f, 0.5f, 0.0f), 1, m3));
    scene->Add(Sphere(Vector(1.2f, 0.0f, 0.0f), 1, m3));
    scene->Add(Sphere(Vector(2.0f, 1.0f, 2.0f), 1, m1));
    
    scene->Add(Light{
        .position = Vector(0,10,10),
        .ambient = Vector(0.4, 0.4, 0.4),
        .diffuse = Vector(0.8, 0.8, 0.8),
        .specular = Vector(1, 1, 1)
    });
    
    scene->Add(Light{
        .position = Vector(10,-10,-10),
        .ambient = Vector(0.4, 0.4, 0.4),
        .diffuse = Vector(0.8, 0.8, 0.8),
        .specular = Vector(1, 1, 1)
    });
}

void transparencyCase(Scene* scene) {
    Material m1 = Material{
        .ambient = Vector(0, 0, 0.5),
        .diffuse = Vector(0.6, 0.4, 1.0),
        .specular = Vector(1.0, 1.0, 1.0),
        .shininess = 5,
        .reflective = true,
        .transparency = false,
        .refractionIndex = 0.5
    };
    scene->Add(Sphere(Vector(0.0f, 0.0f, -5.0f), 1, m1));

    Material m2 = Material{
        .ambient = Vector(0.5, 0, 0),
        .diffuse = Vector(0.5, 0, 0),
        .specular = Vector(1, 1, 1),
        .shininess = 5,
        .reflective = false,
        .transparency = true,
        .refractionIndex = 0.1
    };
    scene->Add(Sphere(Vector(0.0f, 0.0f, 2.0f), 3, m2));
    
    scene->Add(Light{
        .position = Vector(0,10,10),
        .ambient = Vector(0.4, 0.4, 0.4),
        .diffuse = Vector(0.8, 0.8, 0.8),
        .specular = Vector(1, 1, 1)
    });
}

int main(int argc, char **argv) {
    cout << "There are " << omp_get_num_procs() << " procs." << endl;
    init(argc, argv);
    glutMainLoop();
    return 0;
}
