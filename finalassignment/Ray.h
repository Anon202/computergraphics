#ifndef _RAY_H_
#define _RAY_H_

#include "Vec3.h"

typedef struct _HitRec {
    float tHit;
    int primIndex;
    Vec3f p;
    Vec3f n;
    bool anyHit;
    Vec3f color;
} HitRec;

class Ray {
public:
	Vec3f o, d;
	float tClip;	
	static float rayEps;
	static float tMax;
public:
	Ray();	
	Ray(const Vec3f& o, const Vec3f& d, float tclip = tMax);
	void EpsMoveStartAlongSurfaceNormal(const Vec3f& n);
	void EpsMoveStartAlongDir();
	static void IncRayEps();
	static void DecRayEps();
};

#endif
