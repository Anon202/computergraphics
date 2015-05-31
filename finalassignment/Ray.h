#ifndef _RAY_H_
#define _RAY_H_

#include <random>
#include "../algebra/Vector.h"

using namespace std;
using namespace algebra;

typedef struct _HitRec {
    float tHit;
    int primIndex;
    Vector p;
    Vector n;
    bool anyHit;
} HitRec;

class Ray {
public:
	Vector o, d;
	float tClip;	
	static float rayEps;
	static float tMax;
    static mt19937 rnd_gen;
    static uniform_real_distribution<float> distribution;
public:
	Ray();	
	Ray(const Vector& o, const Vector& d, float tclip = tMax);
	void EpsMoveStartAlongSurfaceNormal(const Vector& n);
	void EpsMoveStartAlongDir();
    void RandomlyMoveDirection();
	static void IncRayEps();
	static void DecRayEps();
};

#endif
