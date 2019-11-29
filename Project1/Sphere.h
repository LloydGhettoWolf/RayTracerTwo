#pragma once
#include "Primitive.h"
#include <math.h>

class Material;

class Sphere : public Primitive
{
public:
	Sphere(const Vector3 &center, float r, Material *matPtr, const Vector3 &camOrigin) : mCenter(center), mOC(camOrigin - mCenter), mRadius(r), mMatPtr(matPtr) { mRadiusSq = mRadius * mRadius;};

	~Sphere() 
	{ 
		if (mMatPtr != nullptr) 
			delete mMatPtr; 
	}

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const override;
private:
	Vector3 mCenter;
	int padding = 0;
	Vector3 mOC;
	int padding2 = 0;
	Material* mMatPtr;
	float mRadius;
	float mRadiusSq;
};