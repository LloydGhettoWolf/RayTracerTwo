#include "AABB.h"
#include "Primitive.h"


bool AABB::Hit(const Ray& r, float tMin, float tMax) const
{
	const Vector3& origin = r.GetOrigin();
	const Vector3& dir = r.GetDir();

	for (int axis = 0; axis < 3; axis++)
	{
		float t0 = fminf((mMin[axis] - origin[axis]) / dir[axis],
				(mMax[axis] - origin[axis]) / dir[axis]);
		float t1 = fmaxf((mMin[axis] - origin[axis]) / dir[axis],
						 (mMax[axis] - origin[axis]) / dir[axis]);

		tMin = fmaxf(t0, tMin);
		tMax = fminf(t1, tMax);

		if (tMax <= tMin)
			return false;
	}

	return true;
}

AABB SurroundingBox(AABB box0, AABB box1)
{
	Vector3 small(fminf(box0.GetMin()[X], box1.GetMin()[X]),
				  fminf(box0.GetMin()[Y], box1.GetMin()[Y]),
		          fminf(box0.GetMin()[Z], box1.GetMin()[Z]));
	Vector3 big(fmaxf(box0.GetMax()[X], box1.GetMax()[X]),
				fmaxf(box0.GetMax()[Y], box1.GetMax()[Y]),
				fmaxf(box0.GetMax()[Z], box1.GetMax()[Z]));
	return AABB(small, big);
}

int BoxXCompare(const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Primitive* ah = *(Primitive **)a;
	Primitive* bh = *(Primitive **)b;

	if ((boxLeft.GetMin()[X] - boxRight.GetMin()[X]) < 0.0f)
	{
		return -1;
	}

	return 1;
}

int BoxYCompare(const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Primitive* ah = *(Primitive * *)a;
	Primitive* bh = *(Primitive * *)b;

	if ((boxLeft.GetMin()[Y] - boxRight.GetMin()[Y]) < 0.0f)
	{
		return -1;
	}

	return 1;
}

int BoxZCompare(const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Primitive* ah = *(Primitive * *)a;
	Primitive* bh = *(Primitive * *)b;

	if ((boxLeft.GetMin()[Z] - boxRight.GetMin()[Z]) < 0.0f)
	{
		return -1;
	}

	return 1;
}