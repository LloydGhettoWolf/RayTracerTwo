#pragma once

#include "Ray.h"
#include "NodeType.h"

class Material;
class OctreeNode;

class Primitive : public NodeType
{
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const { return true; };
	virtual bool IntersectWithAABB(OctreeNode* aabb) const { return true;}
	virtual ~Primitive() {};
};