#ifndef _SIMPLE_RAY_TRACER_H
#define _SIMPLE_RAY_TRACER_H

#include "Ray.h"
#include "Scene.h"
#include "Image.h"
#include "../algebra/Vector.h"
#include "../common/Camera.h"

class SimpleRayTracer {
private:
    Scene* scene;
    long long int tests_done;

public:
    Image* image;
    Camera cam;
    bool benchmark;
    SimpleRayTracer(Scene* scene, Image* image, Camera cam);
    void FireRays(void (*glSetPixel)(int, int, const Vector&));
    Image* GetImage(void);
    Color CastRay(const Ray& ray, int depth, int ignore);
    Color Lightning(Vector rayOrigin, const HitRec& hitRec, int depth);
    Vector GetEyeRayDirection(float x, float y);
    HitRec SearchClosestHit(const Ray& ray, int ignore);
};

#endif
