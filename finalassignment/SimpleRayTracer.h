#ifndef _SIMPLE_RAY_TRACER_H
#define _SIMPLE_RAY_TRACER_H

#include "Ray.h"
#include "Scene.h"
#include "Image.h"
#include "Vec3.h"

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

public:
    SimpleRayTracer(Scene* scene, Image* image);
    void SearchClosestHit(const Ray& ray, HitRec& hitRec);
    void FireRays(void (*glSetPixel)(int, int, const Vec3f&));
    Image* GetImage(void);
};

#endif
