#include "Rectangle.h"
#include "Materials.h"


Rectangle::Rectangle(float dim10, float dim11, float dim20, float dim21, float k, Material* mat, RECT_TYPE type) : mDim10(dim10), mDim11(dim11), mDim20(dim20), mDim21(dim21),
mK(k), mMatPtr(mat), mType(type)
{};

bool Rectangle::Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const
{
	int index = (int)mType;
	const Vector3& origin = r.GetOrigin();
	const Vector3& dir    = r.GetDir();

	float t = (mK - origin[mType]) / dir[mType];

	if (t < tMin || t > tMax)
		return false;

	// get indices of sides needed
	int dim1 = (index + 1) % 3;
	int dim2 = (index + 2) % 3;

	float first  = origin[dim1] + t * dir[dim1];
	float second = origin[dim2] + t * dir[dim2];

	if (first < mDim10 || first > mDim11 || second < mDim20 || second > mDim21)
		return false;

	record.t = t;
	record.matPtr = mMatPtr;
	record.point = origin + dir * t;
	Vector3 normal = Vector3(0.0f);
	normal[(int)mType] = 1.0f;
	record.normal = normal;

	return true;
}

bool Rectangle::BoundingBox(float t0, float t1, AABB& box) const
{
	box = AABB(Vector3(mDim10, mDim11, mK - 0.0001f), Vector3(mDim20, mDim21, mK + 0.0001f));
	return true;
}