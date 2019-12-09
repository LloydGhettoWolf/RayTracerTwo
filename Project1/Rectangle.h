#pragma once
#include "Primitive.h"

class Material;

enum RECT_TYPE {YZ = 0, XZ, XY};

class Rectangle : public Primitive
{
public:
	Rectangle(float dim10, float dim11, float dim20, float dim21, float k, Material* mat, RECT_TYPE type, bool flip = false);

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;
	Material* GetMaterial() const { return mMatPtr; }
private:
	float mDim10, mDim11, mDim20, mDim21, mK;
	Material* mMatPtr;
	RECT_TYPE mType;
	bool mFlip;
};