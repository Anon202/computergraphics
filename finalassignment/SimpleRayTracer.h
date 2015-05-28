#ifndef _SIMPLE_RAY_TRACER_H
#define _SIMPLE_RAY_TRACER_H

#include "Ray.h"
#include "Scene.h"
#include "Image.h"
#include "../algebra/Vector.h"
#include "Camera.h"

class SimpleRayTracer {
private:
    Scene* scene;
    Image* image;
    Vector GetEyeRayDirection(int x, int y);
    int tests_done;
    HitRec SearchClosestHit(const Ray& ray, int ignore);

public:
    Camera cam;
    SimpleRayTracer(Scene* scene, Image* image);
    void FireRays(void (*glSetPixel)(int, int, const Vector&));
    Image* GetImage(void);
    Color CastRay(const Ray& ray, int depth);
    Color Lightning(Vector rayOrigin, const HitRec& hitRec, int depth);
};

#endif
