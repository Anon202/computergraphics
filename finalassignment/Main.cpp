#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glut.h>

#include "Vec3.h"
#include "Image.h"
#include "Sphere.h"
#include "Scene.h"
#include "SimpleRayTracer.h"

using namespace std;

void glSetPixel(int x, int y, const Vec3f & c) {
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

SimpleRayTracer *rayTracer;

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
    scene->Add(Sphere(Vec3f(-0.5f, 0.0f, -5.0f), 0.5f, Color{1,0,0}));
    scene->Add(Sphere(Vec3f(0.0f, 0.5f, -6.0f), 0.5f, Color{0,1,0}));
    scene->Add(Sphere(Vec3f(0.4f, 0.0f, -7.0f), 0.5f, Color{0,0,1}));

    Image *image = new Image(640, 480);

    rayTracer = new SimpleRayTracer(scene, image);

}

int main(int argc, char **argv) {
    init(argc, argv);
    glutMainLoop();
    return 0;
}
