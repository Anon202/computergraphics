#include "SimpleRayTracer.h"
#include <limits>
#include <iostream>
#include <cmath>

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

Color SimpleRayTracer::Lightning(const Ray& ray, const HitRec& hitRec) {
    Sphere sphere = this->scene->spheres[hitRec.primIndex]; 
    Light light = Light{
        .position = Vec3f(-5, -3, 1),
        .ambient = Vec3f(0.4, 0.4, 0.4),
        .diffuse = Vec3f(0.8, 0.8, 0.8),
        .specular = Vec3f(1, 1, 1)
    };

    Vec3f v = (-hitRec.p).normalize();
    Vec3f n = (hitRec.n * 1).normalize();
    Vec3f l = (hitRec.p - light.position).normalize();
    Vec3f r =  (l - n*2.0*n.dot(l)).normalize(); // reflect(-l, n)
    Color ambient = sphere.ambient.multCoordwise(light.ambient);
    Color diffuse = sphere.diffuse.multCoordwise(light.diffuse) * max(n.dot(l), 0.0f);
    Color specular = sphere.specular.multCoordwise(light.specular) *
                     pow(max(r.dot(v), 0.0f), sphere.shininess);
    return ambient + diffuse + specular;
}

HitRec SimpleRayTracer::SearchClosestHit(const Ray& ray) {
    float maxZ = -std::numeric_limits<float>::max();
    HitRec closestHit;
    HitRec hitRec;
    closestHit.anyHit = false;
    for (unsigned int i = 0; i < this->scene->spheres.size(); i++) {
        this->tests_done++;
        if (this->scene->spheres[i].Hit(ray, hitRec) && this->scene->spheres[i].c.z > maxZ) {
            this->scene->spheres[i].ComputeSurfaceHitFields(ray, hitRec);
            closestHit = hitRec;
            maxZ = this->scene->spheres[i].c.z;
            closestHit.primIndex = i;
        }
    }
    return closestHit;
}

Color SimpleRayTracer::CastRay(const Ray& ray) {
    HitRec hitRec = SearchClosestHit(ray);
    if (hitRec.anyHit) {
        return Lightning(ray, hitRec); 
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
            Color color = this->CastRay(ray);
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
