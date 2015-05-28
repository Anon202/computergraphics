#include "SimpleRayTracer.h"
#include <limits>
#include <iostream>
#include <cmath>

#define MAX_DEPTH 3

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

Color SimpleRayTracer::Lightning(Vec3f rayOrigin, const HitRec& hitRec, int depth) {
    Sphere sphere = this->scene->spheres[hitRec.primIndex]; 
    Color color = Color(0,0,0);
    for (unsigned int i = 0; i < this->scene->lights.size(); i++) {
        Light light = this->scene->lights[i];

        Vec3f v = (rayOrigin - hitRec.p).normalize();
        Vec3f n = (hitRec.n * 1).normalize();
        Vec3f l = (light.position - hitRec.p).normalize();
        Vec3f r = (n*2.0*n.dot(l) - l).normalize(); // reflect(-l, n)
        Color ambient = sphere.ambient.multCoordwise(light.ambient);
        Color diffuse = sphere.diffuse.multCoordwise(light.diffuse) * max(n.dot(l), 0.0f);
        Color specular = sphere.specular.multCoordwise(light.specular) *
                         pow(max(r.dot(v), 0.0f), sphere.shininess);
        
        Ray newRay;
        newRay.o = hitRec.p;
        newRay.d = r;
        newRay.EpsMoveStartAlongSurfaceNormal(n);
        Color ref = this->CastRay(newRay, depth + 1) * pow(max(r.dot(v), 0.0f), sphere.shininess);
        
        Ray shadowRay;
        shadowRay.o = hitRec.p;
        shadowRay.d = l;
        HitRec shadowHitRec = this->SearchClosestHit(shadowRay, hitRec.primIndex);
        if (shadowHitRec.anyHit) {
            color += ambient + ref;
            continue;
        }
        color += ambient + diffuse + specular + ref;
    }
    return color;
}

HitRec SimpleRayTracer::SearchClosestHit(const Ray& ray, int ignore=-1) {
    HitRec closestHit;
    closestHit.tHit = std::numeric_limits<float>::max();
    HitRec hitRec;
    closestHit.anyHit = false;
    for (int i = 0; i < (int)this->scene->spheres.size(); i++) {
        if (i == ignore) {
            continue;
        }
        this->tests_done++;
        if (this->scene->spheres[i].Hit(ray, hitRec) && hitRec.tHit < closestHit.tHit) {
            closestHit = hitRec;
            closestHit.primIndex = i;
        }
    }
    return closestHit;
}

Color SimpleRayTracer::CastRay(const Ray& ray, int depth) {
    if (depth > MAX_DEPTH) {
        return Color(0,0,0);
    }
    HitRec hitRec = SearchClosestHit(ray);
    if (hitRec.anyHit) {
        return Lightning(ray.o, hitRec, depth); 
    } else {
        return Color(0,0,0);
    }
}

void SimpleRayTracer::FireRays(void (*glSetPixel)(int, int, const Vec3f&)) {
    this->tests_done = 0;
    Ray ray;
    ray.o = this->cam.position; //Set the start position of the eye rays to the origin 

    for (int y = 0; y < this->image->GetHeight(); y++) {
        for (int x = 0; x < this->image->GetWidth(); x++) {
            ray.d = this->GetEyeRayDirection(x, y);
            Color color = this->CastRay(ray, 1);
            this->image->SetPixel(x, y, color);
            glSetPixel(x, y, color);
        }
    }
}

Image* SimpleRayTracer::GetImage(void) {
    return this->image;
}

int SimpleRayTracer::TestsDone(void) {
    return this->tests_done;
}
