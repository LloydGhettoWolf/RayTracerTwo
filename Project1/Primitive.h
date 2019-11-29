#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;
class OctreeNode;

struct HitRecord {
	float t = 99999.99f;
	Vector3 point;
	Vector3 normal;
	Material* matPtr;
};

class Primitive
{
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const = 0;
	virtual ~Primitive() {};
};