#pragma once
#include "Primitive.h"

class Material;

class Box : public Primitive
{
public:
	Box() : mMatPtr(nullptr), mListPtr(nullptr) {};
	Box(const Vector3& min, const Vector3& max, Material* matPtr);

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const override;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const override;

private:
	Vector3 mMin, mMax;
	Material* mMatPtr;
	Primitive* mListPtr;
};