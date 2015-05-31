#include "SimpleRayTracer.h"
#include <limits>
#include <iostream>
#include <cmath>
#include <ctime>

#define MAX_DEPTH 5
#define LIGHT_SAMPLES 3
#define REFLECT_RAYS 1

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
    return this->cam.LookAt() * Vector(left + x * dx, bottom + y * dy, z).Normalized();
}

SimpleRayTracer::SimpleRayTracer(Scene* scene, Image* image, Camera cam) :
    scene(scene), image(image), cam(cam), benchmark(false) {
}

Color SimpleRayTracer::Lightning(Vector rayOrigin, const HitRec& hitRec, int depth) {
    Sphere sphere = this->scene->spheres[hitRec.primIndex]; 
    Material spherem = sphere.material;
    Color color = Color(0,0,0);
    Vector v = (rayOrigin - hitRec.p).Normalized();
    Vector n = hitRec.n.Normalized();
    
    for (unsigned int i = 0; i < this->scene->lights.size(); i++) {
        Light light = this->scene->lights[i];
        int numShadowHits = 0;
        
        for (unsigned int j = 0; j < LIGHT_SAMPLES; j++) { 
            Vector lightpos = this->scene->lights[i].RandomPoint(); 

            Vector l = (lightpos - hitRec.p).Normalized();
            Color ambient = spherem.ambient.MultCoordwise(light.Ambient());
            
            HitRec shadowHitRec;
            Ray shadowRay;
            shadowRay.o = hitRec.p;
            shadowRay.d = l;
            shadowRay.EpsMoveStartAlongSurfaceNormal(n);
            bool hit = false;
            for (int k = 0; k < (int)this->scene->spheres.size(); k++) {
                if (k == hitRec.primIndex) {
                    continue;
                }
                this->tests_done++;
                if (this->scene->spheres[i].Hit(shadowRay, shadowHitRec)) {
                    numShadowHits++;
                    hit = true;
                }
            }
            
            if (hit) {
                color += ambient;
            } else {
                Vector r = (n*2.0*n.Dot(l) - l).Normalized(); // reflect(-l, n)
                Color diffuse = spherem.diffuse.MultCoordwise(light.Diffuse()) *
                                max(n.Dot(l), 0.0f);
                Color specular = spherem.specular.MultCoordwise(light.Specular()) *
                                pow(max(r.Dot(v), 0.0f), spherem.shininess);
                color += ambient + diffuse + specular;
            }
        }
        
        color = color * (1.0f/(float)LIGHT_SAMPLES);
    }
    
    float refCoeff = 0.4;

    if (spherem.reflective) {
        Color refColor = Color(0,0,0);
        for (int i = 0; i < REFLECT_RAYS; i++) {
            Ray reflectRay;
            reflectRay.o = hitRec.p;
            reflectRay.d = n * 2.0 * v.Dot(n) - v;
            //reflectRay.RandomlyMoveDirection();
            reflectRay.d = reflectRay.d.Normalized();
            reflectRay.EpsMoveStartAlongSurfaceNormal(n);
            refColor += this->CastRay(reflectRay, depth + 1, hitRec.primIndex) * refCoeff;
        }
        color += refColor * (1.0f/(float)REFLECT_RAYS);
    }

    if (spherem.transparency) {
        float ni = (n.Dot(v) < 0)? spherem.refractionIndex : 1.0;
        float nt = (n.Dot(v) < 0)? 1.0 : spherem.refractionIndex;
        float nr = ni / nt;
        float cosI = n.Dot(v);
        float cosT2 = 1.0 - nr*nr*(1.0 - cosI*cosI);
        if (cosT2 > 0.0) {
            Ray refractRay;
            refractRay.o = hitRec.p;
            refractRay.d = (n*(nr*cosI - sqrt(cosT2)) - v*nr).Normalized();
            n = (n.Dot(v) < 0)? n : -n;
            refractRay.EpsMoveStartAlongSurfaceNormal(n);
            color += this->CastRay(refractRay, depth + 1, hitRec.primIndex) * (1 - refCoeff);
        }
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

Color SimpleRayTracer::CastRay(const Ray& ray, int depth, int ignore = -1) {
    if (depth > MAX_DEPTH) {
        return Color(0,0,0);
    }
    HitRec hitRec = SearchClosestHit(ray, ignore);
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
            glSetPixel(x, y, this->image->GetPixel(x, y));
        }
    }
    benchmark = true;   
    if (benchmark) {
        float ms = (float)(clock() - t)/CLOCKS_PER_SEC * 1000;
        printf("Fire rays time: %.4fms, ray-sphere intersection tests: %d\n",
                ms, this->tests_done);
    }
}

Image* SimpleRayTracer::GetImage(void) {
    return this->image;
}
