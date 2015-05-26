#ifndef _SIMPLE_RAY_TRACER_H
#define _SIMPLE_RAY_TRACER_H

#include "Ray.h"
#include "Scene.h"
#include "Image.h"
#include "Vec3.h"
#include "Camera.h"

typedef struct _Light {
    Vec3f position;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
} Light;

class SimpleRayTracer {
private:
    Scene* scene;
    Image* image;
    Vec3f GetEyeRayDirection(int x, int y);
    int tests_done;
    HitRec SearchClosestHit(const Ray& ray);

public:
    Camera cam;
    SimpleRayTracer(Scene* scene, Image* image);
    void FireRays(void (*glSetPixel)(int, int, const Vec3f&));
    Image* GetImage(void);
    int TestsDone(void);
    Color CastRay(int x, int y, const Ray& ray);
};

#endif
