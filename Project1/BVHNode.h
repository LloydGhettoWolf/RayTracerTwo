#pragma once
#include "Primitive.h"

class BVHNode : public Primitive
{
public:
	BVHNode() {};
	BVHNode(Primitive** list, int n, float t0, float t1);
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const override;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const override;

private:
	Primitive* mLeft = nullptr;
	Primitive* mRight = nullptr;
	AABB mBox;
};