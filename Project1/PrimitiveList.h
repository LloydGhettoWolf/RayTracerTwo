#pragma once

#include "Primitive.h"


class PrimitiveList : public Primitive
{
public:
	PrimitiveList() {};
	PrimitiveList(Primitive** list, int n) : mList(list), mSize(n) {};
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;
	Primitive** GetList() { return mList; }
	int GetSize() const { return mSize; }
	Material* GetMaterial() const { return nullptr; }
private:
	Primitive** mList = nullptr;
	int mSize = 0;
};

