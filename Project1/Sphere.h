#pragma once
#include "Primitive.h"
#include <math.h>

class Material;

class Sphere : public Primitive
{
public:
	Sphere(const Vector3& center, float r, Material* matPtr) : mCenter(center), mRadius(r), mMatPtr(matPtr) { mRadiusSq = mRadius * mRadius; mInvRadius = 1.0f / mRadius; };

	~Sphere() 
	{ 
		if (mMatPtr != nullptr) 
			delete mMatPtr; 
	}

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const override;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const override;
private:
	Vector3 mCenter;
	Material* mMatPtr;
	float mRadius;
	float mRadiusSq;
	float mInvRadius;
};