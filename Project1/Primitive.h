#pragma once

#include "Ray.h"

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
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const { return true; };
	virtual ~Primitive() {};
};