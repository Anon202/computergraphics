#include "SimpleRayTracer.h"
#include <limits>
#include <iostream>
#include <cmath>
#include <ctime>

#define NUM_THREADS 4
#define MAX_DEPTH 5
#define LIGHT_SAMPLES 50
#define REFLECT_RAYS 20
#define NUM_SAMPLES 3

using namespace std;
using namespace algebra;

Vector SimpleRayTracer::GetEyeRayDirection(float x, float y) {
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
    Shape* shape = this->scene->shapes[hitRec.primIndex]; 
    Material shapem = shape->material;
    Color color = Color(0,0,0);
    Vector v = (rayOrigin - hitRec.p).Normalized();
    Vector n = hitRec.n.Normalized();
    
    for (unsigned int i = 0; i < this->scene->lights.size(); i++) {
        Light light = this->scene->lights[i];
        
        Color c = Color(0,0,0);

        for (unsigned int j = 0; j < LIGHT_SAMPLES; j++) { 
            Vector lightpos = this->scene->lights[i].RandomPoint(); 

            Vector l = (lightpos - hitRec.p).Normalized();
            Color ambient = shape->GetColor(hitRec.p).MultCoordwise(light.Ambient());

            HitRec shadowHitRec;
            Ray shadowRay;
            shadowRay.o = hitRec.p;
            shadowRay.d = l;
            shadowRay.EpsMoveStartAlongSurfaceNormal(n);
            bool hit = false;
            for (int k = 0; k < (int)this->scene->shapes.size(); k++) {
                if (k == hitRec.primIndex) {
                    continue;
                }
                this->tests_done++;
                if (this->scene->shapes[k]->Hit(shadowRay, shadowHitRec)) {
                    hit = true;
                    break;
                }
            }
            
            if (hit) {
                c += ambient;
            } else {
                Vector r = (n*2.0*n.Dot(l) - l).Normalized(); // reflect(-l, n)
                Color diffuse = shapem.diffuse.MultCoordwise(light.Diffuse()) *
                                max(n.Dot(l), 0.0f);
                Color specular = shapem.specular.MultCoordwise(light.Specular()) *
                                pow(max(r.Dot(v), 0.0f), shapem.shininess);
                c += ambient + diffuse + specular;
            }
        }
        
        color = color + c * (1.0f/(float)LIGHT_SAMPLES);
    }
    
    float refCoeff = 0.4;

    if (shapem.reflective) {
        Color refColor = Color(0,0,0);
        int iters = (shapem.blurDegree > 0.0)? REFLECT_RAYS : 1;
        for (int i = 0; i < iters; i++) {
            Ray reflectRay;
            reflectRay.o = hitRec.p;
            reflectRay.d = (n * 2.0 * v.Dot(n) - v).Normalized();
            if (shapem.blurDegree > 0.0) {
                reflectRay.RandomlyPerturbDirection(shapem.blurDegree);
            }
            reflectRay.EpsMoveStartAlongSurfaceNormal(n);
            refColor += this->CastRay(reflectRay, depth + 1, hitRec.primIndex) * refCoeff;
        }
        color += refColor * (1.0f/(float)iters);
    }

    if (shapem.transparency) {
        float ni = (n.Dot(v) < 0)? shapem.refractionIndex : 1.0;
        float nt = (n.Dot(v) < 0)? 1.0 : shapem.refractionIndex;
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
    for (int i = 0; i < (int)this->scene->shapes.size(); i++) {
        if (i == ignore) {
            continue;
        }
        this->tests_done++;
        if (this->scene->shapes[i]->Hit(ray, hitRec) && hitRec.tHit < closestHit.tHit) {
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

pthread_mutex_t numThreadsMutex = PTHREAD_MUTEX_INITIALIZER;
int threadCount = 0;

void *run_thread(void *arg) {
    SimpleRayTracer *tracer = (SimpleRayTracer*)arg;
    pthread_mutex_lock(&numThreadsMutex);
    int threadNum = threadCount++;
    pthread_mutex_unlock(&numThreadsMutex);
    for (int y = 0; y < tracer->image->GetHeight(); y++) {
        for (int x = 0; x < tracer->image->GetWidth(); x++) {
            if ((x * tracer->image->GetWidth() + y) % NUM_THREADS == threadNum) { 
                Color color = Color(0,0,0);
                for (int yy = 0; yy < NUM_SAMPLES; yy++) {
                    for (int xx = 0; xx < NUM_SAMPLES; xx++) {
                        float u = x + (xx + 0.5)/NUM_SAMPLES;
                        float v = y + (yy + 0.5)/NUM_SAMPLES;
                        Ray ray;
                        ray.o = tracer->cam.Position();
                        ray.d = tracer->GetEyeRayDirection(u, v);
                        color += tracer->CastRay(ray, 1);
                        //cout << x << " " << y << " -> " << u << " " << v << endl;
                    }
                }
                tracer->image->SetPixel(x, y, color * (1.0/(NUM_SAMPLES * NUM_SAMPLES)));
            }
        }
    }
    pthread_exit(NULL);
}

void SimpleRayTracer::FireRays(void (*glSetPixel)(int, int, const Vector&)) {
    this->tests_done = 0;
    clock_t t = clock();

    threadCount = 0;
    pthread_t thread[NUM_THREADS];
    int error;
    for (int i = 0; i < NUM_THREADS; i++) {
        if ((error = pthread_create(&thread[i], NULL, run_thread, (void*)this))){
            cerr << "Failed to create thread #" << i << ",number: " << error << "\n";
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    for (int y = 0; y < this->image->GetHeight(); y++) {
        for (int x = 0; x < this->image->GetWidth(); x++) {
            glSetPixel(x, y, this->image->GetPixel(x, y));
        }
    }

    benchmark = true;
    if (benchmark) {
        float ms = (float)(clock() - t)/CLOCKS_PER_SEC;
        printf("Fire rays time: %.4fs, ray-shape intersection tests: %lld\n",
                ms, this->tests_done);
    }
}

Image* SimpleRayTracer::GetImage(void) {
    return this->image;
}
