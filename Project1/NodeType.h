#pragma once
#include "Ray.h"

class Material;

struct HitRecord {
	float t = 99999.99f;
	Vector3 point;
	Vector3 normal;
	Material* matPtr;
};

class NodeType
{
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const = 0;
};