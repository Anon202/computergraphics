#include "SimpleRayTracer.h"
#include <limits>
#include <iostream>

using namespace std;

Vec3f SimpleRayTracer::GetEyeRayDirection(int x, int y) {
    //Uses a fix camera looking along the negative z-axis
    static float z = -5.0f;
    static float sizeX = 4.0f;
    static float sizeY = 3.0f;
    static float left = -sizeX * 0.5f;
    static float bottom = -sizeY * 0.5f;
    static float dx =  sizeX / float(image->GetWidth());
    static float dy =  sizeY / float(image->GetHeight());
    return Vec3f(left + x * dx, bottom + y * dy, z).normalize();
}

SimpleRayTracer::SimpleRayTracer(Scene* scene, Image* image) {
    this->scene = scene;
    this->image = image;
}

void SimpleRayTracer::SearchClosestHit(const Ray& ray, HitRec& hitRec) {
    float maxZ = -std::numeric_limits<float>::max();
    HitRec closestHit;
    closestHit.anyHit = false;
    for (unsigned int i = 0; i < this->scene->spheres.size(); i++) {
        if (this->scene->spheres[i].Hit(ray, hitRec) && this->scene->spheres[i].c.z > maxZ) {
            closestHit = hitRec;
            closestHit.color = this->scene->spheres[i].color;
            maxZ = this->scene->spheres[i].c.z;
        }
    }
    hitRec = closestHit;
}

void SimpleRayTracer::FireRays(void (*glSetPixel)(int, int, const Vec3f&)) {
    Ray ray;
    HitRec hitRec;
    //bool hit = false;
    ray.o = Vec3f(0.0f, 0.0f, 0.0f); //Set the start position of the eye rays to the origin

    for (int y = 0; y < this->image->GetHeight(); y++) {
        for (int x = 0; x < this->image->GetWidth(); x++) {
            ray.d = this->GetEyeRayDirection(x, y);
            hitRec.anyHit = false;
            SearchClosestHit(ray, hitRec);
            if (hitRec.anyHit) {
                this->image->SetPixel(x, y, hitRec.color);
                glSetPixel(x, y, hitRec.color);
            } else {
                this->image->SetPixel(x, y, Vec3f(0.0f, 0.0f, 0.0f));
                glSetPixel(x, y, Vec3f(0.0f, 0.0f, 0.0f));
            }
        }
    }
}

Image* SimpleRayTracer::GetImage(void) {
    return this->image;
}
