#include "SimpleRayTracer.h"
#include <limits>
#include <iostream>
#include <cmath>
#include <ctime>

#define MAX_DEPTH 3

using namespace std;
using namespace algebra;

Vector SimpleRayTracer::GetEyeRayDirection(int x, int y) {
    //Uses a fix camera looking along the negative z-axis
    static float z = -5.0f;
    static float sizeX = 4.0f;
    static float sizeY = 3.0f;
    static float left = -sizeX * 0.5f;
    static float bottom = -sizeY * 0.5f;
    static float dx =  sizeX / float(image->GetWidth());
    static float dy =  sizeY / float(image->GetHeight());
    return Vector(left + x * dx, bottom + y * dy, z).Normalized();
}

SimpleRayTracer::SimpleRayTracer(Scene* scene, Image* image, Camera cam) :
    scene(scene), image(image), cam(cam) {
}

Color SimpleRayTracer::Lightning(Vector rayOrigin, const HitRec& hitRec, int depth) {
    Sphere sphere = this->scene->spheres[hitRec.primIndex]; 
    Color color = Color(0,0,0);
    for (unsigned int i = 0; i < this->scene->lights.size(); i++) {
        Light light = this->scene->lights[i];

        Vector v = (rayOrigin - hitRec.p).Normalized();
        Vector n = (hitRec.n * 1).Normalized();
        Vector l = (light.position - hitRec.p).Normalized();
        Vector r = (n*2.0*n.Dot(l) - l).Normalized(); // reflect(-l, n)
        Color ambient = sphere.ambient.MultCoordwise(light.ambient);
        Color diffuse = sphere.diffuse.MultCoordwise(light.diffuse) * max(n.Dot(l), 0.0f);
        Color specular = sphere.specular.MultCoordwise(light.specular) *
                         pow(max(r.Dot(v), 0.0f), sphere.shininess);
        
        Ray shadowRay;
        shadowRay.o = hitRec.p;
        shadowRay.d = l;
        HitRec shadowHitRec = this->SearchClosestHit(shadowRay, hitRec.primIndex);
        if (shadowHitRec.anyHit) {
            color += ambient;
            continue;
        }
        
        Ray newRay;
        newRay.o = hitRec.p;
        newRay.d = r;
        newRay.EpsMoveStartAlongSurfaceNormal(n);
        Color ref = this->CastRay(newRay, depth + 1) * pow(max(r.Dot(v), 0.0f), sphere.shininess);
        
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

void SimpleRayTracer::FireRays(void (*glSetPixel)(int, int, const Vector&)) {
    this->tests_done = 0;
    clock_t t = clock();

    for (int y = 0; y < this->image->GetHeight(); y++) {
        for (int x = 0; x < this->image->GetWidth(); x++) {
            Ray ray;
            ray.o = this->cam.Position();
            ray.d = this->GetEyeRayDirection(x, y);
            Color color = this->CastRay(ray, 1);
            this->image->SetPixel(x, y, color);
        }
    }
    
    float ms = (float)(clock() - t)/CLOCKS_PER_SEC * 1000;
    printf("Fire rays time: %.4fms, ray-sphere intersection tests: %d\n",
            ms, this->tests_done);
    
    for (int y = 0; y < this->image->GetHeight(); y++) {
        for (int x = 0; x < this->image->GetWidth(); x++) {
            glSetPixel(x, y, this->image->GetPixel(x, y));
        }
    }
}

Image* SimpleRayTracer::GetImage(void) {
    return this->image;
}
