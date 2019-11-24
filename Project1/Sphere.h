#pragma once
#include "Primitive.h"
#include "OctreeNode.h"
#include <math.h>

class Material;

class Sphere : public Primitive
{
public:
	Sphere(Vector3 center, float r, Material *matPtr) : mCenter(center), mRadius(r), mMatPtr(matPtr) { mRadiusSq = mRadius * mRadius;};

	~Sphere() 
	{ 
		if (mMatPtr != nullptr) 
			delete mMatPtr; 
	}

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const;
	virtual bool IntersectWithAABB(OctreeNode* aabb) const;
private:
	Vector3 mCenter;
	int padding = 0;
	Material* mMatPtr;
	float mRadius;
	float mRadiusSq;
};