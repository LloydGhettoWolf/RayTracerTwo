#include "Box.h"
#include "PrimitiveList.h"
#include "Rectangle.h"

Box::Box(const Vector3& min, const Vector3& max, Material* matPtr)
{
	mMin = min;
	mMax = max;
	mMatPtr = matPtr;

	Primitive** list = new Primitive*[6];

	list[0] = new Rectangle(mMin[X], mMax[X], mMin[Y], mMax[Y], mMax[Z], matPtr, RECT_TYPE::XY);
	list[1] = new Rectangle(mMin[X], mMax[X], mMin[Y], mMax[Y], mMin[Z], matPtr, RECT_TYPE::XY);

	list[2] = new Rectangle(mMin[X], mMax[X], mMin[Z], mMax[Z], mMax[Y], matPtr, RECT_TYPE::XZ);
	list[3] = new Rectangle(mMin[X], mMax[X], mMin[Z], mMax[Z], mMin[Y], matPtr, RECT_TYPE::XZ);

	list[4] = new Rectangle(mMin[Y], mMax[Y], mMin[Z], mMax[Z], mMax[X], matPtr, RECT_TYPE::YZ, true);
	list[5] = new Rectangle(mMin[Y], mMax[Y], mMin[Z], mMax[Z], mMin[X], matPtr, RECT_TYPE::YZ, true);

	mListPtr = new PrimitiveList(list, 6);
}

bool Box::Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const
{
	bool success = mListPtr->Hit(r, tMin, tMax, record, depth);
	return success;
}

bool Box::BoundingBox(float t0, float t1, AABB& box) const
{
	box = AABB(mMin, mMax);
	return true;
}