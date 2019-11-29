#pragma once

#include "Vector3.h"
#include "Ray.h"

class AABB
{
public:
	AABB() {};
	AABB(const Vector3& min, const Vector3& max) : mMin(min), mMax(max) {};

	const Vector3& GetMin() const { return mMin; }
	const Vector3& GetMax() const { return mMax; }

	bool Hit(const Ray& r, float tMin, float tMax) const;
private:
	Vector3 mMin;
	Vector3 mMax;
};

AABB SurroundingBox(AABB box0, AABB box1);

int BoxXCompare(const void* a, const void* b);
int BoxYCompare(const void* a, const void* b);
int BoxZCompare(const void* a, const void* b);

//inline float FMin(float a, float b)
//{
//	return a < b ? a : b;
//}
//
//inline float FMax(float a, float b)
//{
//	return a > b ? a : b;
//}