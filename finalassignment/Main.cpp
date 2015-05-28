#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glut.h>

#include "Vec3.h"
#include "Image.h"
#include "Sphere.h"
#include "Scene.h"
#include "SimpleRayTracer.h"
#include "Camera.h"

using namespace std;

void glSetPixel(int x, int y, const Vec3f & c) {
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

SimpleRayTracer *rayTracer;
void shadowsCase(Scene* scene);
void testCase1(Scene* scene);

void display(void) {
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
    clock_t t;
    float ms;
    switch (key) {
        case 'x': case 'X':
        case 'y': case 'Y':
        case 'z': case 'Z':
            rayTracer->cam.Move(key);
            display();
            break;
        case 's': case 'S':
            rayTracer->GetImage()->Save();
            break;
        case 'm': case 'M':
            t = clock();
            display();
            ms = (float)(clock() - t)/CLOCKS_PER_SEC * 1000;
            printf("Fire rays time: %.4fms, ray-sphere intersection tests: %d\n",
                    ms, rayTracer->TestsDone());
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
    testCase1(scene);

    Image *image = new Image(640, 480);

    rayTracer = new SimpleRayTracer(scene, image);
    rayTracer->cam = Camera(Vec3f(0,0,10), Vec3f(0,1,0), Vec3f(0,0,0));
}

void testCase1(Scene* scene) { 
    scene->Add(Sphere(Vec3f(-1.0f, 0.5f, -6.0f), 0.5f));
    scene->spheres[0].ambient = Vec3f(0, 0, 0.5);
    scene->spheres[0].diffuse = Vec3f(0.6, 0.4, 1.0);
    scene->spheres[0].specular = Vec3f(1.0, 1.0, 1.0);
    scene->spheres[0].shininess = 5;

    scene->Add(Sphere(Vec3f(0.0f, 0.0f, -5.0f), 0.5f));
    scene->spheres[1].ambient = Vec3f(0.5, 0, 0);
    scene->spheres[1].diffuse = Vec3f(0.5, 0, 0);
    scene->spheres[1].specular = Vec3f(1, 1, 1);
    scene->spheres[1].shininess = 5;
    
    scene->Add(Sphere(Vec3f(0.5f, 0.0f, -4.0f), 0.5f));
    scene->spheres[2].ambient = Vec3f(0, 0.5, 0);
    scene->spheres[2].diffuse = Vec3f(0, 0.5, 0);
    scene->spheres[2].specular = Vec3f(1.0, 1.0, 1.0);
    scene->spheres[2].shininess = 5;
    
    scene->Add(Sphere(Vec3f(1.0f, 0.0f, -10.0f), 3.0f));
    scene->spheres[3].ambient = Vec3f(0.8, 0.8, 0);
    scene->spheres[3].diffuse = Vec3f(0.3, 0.3, 0);
    scene->spheres[3].specular = Vec3f(1.0, 1.0, 1.0);
    scene->spheres[3].shininess = 10;

    scene->Add(Light{
        .position = Vec3f(10, 5, -5),
        .ambient = Vec3f(0.4, 0.4, 0.4),
        .diffuse = Vec3f(0.8, 0.8, 0.8),
        .specular = Vec3f(1, 1, 1)
    });
}

void shadowsCase(Scene* scene) {
    scene->Add(Sphere(Vec3f(-1.0f, 0.0f, 2), 1));
    scene->spheres[0].ambient = Vec3f(0, 0, 0.5);
    scene->spheres[0].diffuse = Vec3f(0.6, 0.4, 1.0);
    scene->spheres[0].specular = Vec3f(1.0, 1.0, 1.0);
    scene->spheres[0].shininess = 5;

    scene->Add(Sphere(Vec3f(1.5f, 0.0f, 2), 1));
    scene->spheres[1].ambient = Vec3f(0.5, 0, 0);
    scene->spheres[1].diffuse = Vec3f(0.5, 0, 0);
    scene->spheres[1].specular = Vec3f(1, 1, 1);
    scene->spheres[1].shininess = 5;

    scene->Add(Light{
        .position = Vec3f(-10,10,2),
        .ambient = Vec3f(0.4, 0.4, 0.4),
        .diffuse = Vec3f(0.8, 0.8, 0.8),
        .specular = Vec3f(1, 1, 1)
    });
}

int main(int argc, char **argv) {
    init(argc, argv);
    glutMainLoop();
    return 0;
}
