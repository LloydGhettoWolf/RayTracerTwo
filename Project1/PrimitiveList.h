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
private:
	Primitive** mList = nullptr;
	int mSize = 0;
};

bool PrimitiveList::Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const
{
	HitRecord tempRec;
	bool hitAnything = false;
	float closest = tMax;

	for (int i = 0; i < mSize; i++)
	{
		if (mList[i]->Hit(r, tMin, closest, tempRec, depth)) {
			hitAnything = true;
			closest = tempRec.t;
			record = tempRec;
		}
	}
	return hitAnything;
}

bool PrimitiveList::BoundingBox(float t0, float t1, AABB& box) const
{
	if (mSize < 1)
	{
		return false;
	}

	AABB tempBox;
	bool firstTrue = mList[0]->BoundingBox(t0, t1, tempBox);
	if (!firstTrue) return false;

	for (int i = 0; i < mSize; i++)
	{
		if (mList[0]->BoundingBox(t0, t1, tempBox))
		{
			box = SurroundingBox(box, tempBox);
		}
		else
		{
			return false;
		}
	}

	return true;
}