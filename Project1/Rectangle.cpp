#include "Rectangle.h"
#include "Materials.h"


Rectangle::Rectangle(float dim10, float dim11, float dim20, float dim21, float k, Material* mat, RECT_TYPE type, bool flip) : mDim10(dim10), mDim11(dim11), mDim20(dim20), mDim21(dim21),
mK(k), mMatPtr(mat), mType(type), mFlip(flip)
{};

bool Rectangle::Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const
{
	int index = (int)mType;
	const Vector3& origin = r.GetOrigin();
	const Vector3& dir    = r.GetDir();

	float t = (mK - origin[mType]) / dir[mType];

	if (t < tMin || t > tMax)
		return false;

	int dim1, dim2;

	// get indices of sides needed
	if (mType != RECT_TYPE::XZ)
	{
		dim1 = (index + 1) % 3;
		dim2 = (index + 2) % 3;
	}
	else
	{
		dim1 = index - 1;
		dim2 = index + 1;
	}

	float first  = origin[dim1] + t * dir[dim1];
	float second = origin[dim2] + t * dir[dim2];

	if (first < mDim10 || first > mDim11 || second < mDim20 || second > mDim21)
		return false;

	record.t = t;
	record.matPtr = mMatPtr;
	record.point = origin + dir * t;
	record.prim = (Primitive*)this;

	Vector3 normal = Vector3(0.0f);

	if (mFlip)
	{
		normal[(int)mType] = -1.0f;
	}
	else
	{
		normal[(int)mType] = 1.0f;
	}
	
	record.normal = normal;

	return true;
}

bool Rectangle::BoundingBox(float t0, float t1, AABB& box) const
{
	switch (mType)
	{
		case RECT_TYPE::XY:
			box = AABB(Vector3(mDim10, mDim20, mK - 0.0001f), Vector3(mDim11, mDim21, mK + 0.0001f));
			break;
		case RECT_TYPE::XZ:
			box = AABB(Vector3(mDim10, mK - 0.0001f, mDim20), Vector3(mDim11, mK + 0.0001f, mDim21));
			break;
		case RECT_TYPE::YZ:
			box = AABB(Vector3(mK - 0.0001f, mDim10, mDim20), Vector3(mK + 0.0001f, mDim11, mDim21));
			break;
		default:
			box = AABB();
	}
	
	return true;
}