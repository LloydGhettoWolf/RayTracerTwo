#pragma once

#include "Primitive.h"

class PrimitiveList : public Primitive
{
public:
	PrimitiveList() {};
	PrimitiveList(Primitive** list, int n) : mList(list), mSize(n) {};
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const;
	Primitive** GetList() { return mList; }
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