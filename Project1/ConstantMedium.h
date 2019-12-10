#pragma once
// constant medium for fog, smoke, subsurface etc
#include "Primitive.h"
#include "Materials.h"

class ConstantMedium : public Primitive
{
public:
	ConstantMedium(Primitive* boundary, float density, const Vector3& col) : mBoundary(boundary), mDensity(density) { mPhaseFunction = new Isotropic(col); }
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;
	virtual Material* GetMaterial() const { return mPhaseFunction; };
private:
	Primitive* mBoundary;
	float mDensity;
	Material* mPhaseFunction;
};