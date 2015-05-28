#ifndef _SIMPLE_RAY_TRACER_H
#define _SIMPLE_RAY_TRACER_H

#include "Ray.h"
#include "Scene.h"
#include "Image.h"
#include "Vec3.h"
#include "Camera.h"

class SimpleRayTracer {
private:
    Scene* scene;
    Image* image;
    Vec3f GetEyeRayDirection(int x, int y);
    int tests_done;
    HitRec SearchClosestHit(const Ray& ray, int ignore);

public:
    Camera cam;
    SimpleRayTracer(Scene* scene, Image* image);
    void FireRays(void (*glSetPixel)(int, int, const Vec3f&));
    Image* GetImage(void);
    int TestsDone(void);
    Color CastRay(const Ray& ray, int depth);
    Color Lightning(Vec3f rayOrigin, const HitRec& hitRec, int depth);
};

#endif
