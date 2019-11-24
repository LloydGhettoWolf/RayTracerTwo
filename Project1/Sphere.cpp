#include "Sphere.h"
#include "Globals.h"

bool Sphere::Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const
{
#ifdef GETSTATS
	IncreaseIntersections();
#endif

	const Vector3& rayDir = r.GetDir();
	const Vector3& origin = r.GetOrigin();
	Vector3 oc = origin - mCenter;
	float a = r.GetDot();
	float b = oc.DotProduct(rayDir);
	float c = oc.DotProduct(oc) - mRadiusSq;
	float discriminant = b * b - a * c;

	if (discriminant > 0.0f) {

		float discriminantRoot = sqrtf(discriminant);

		for (int i = 0; i < 2; i++)
		{
			float temp = (-b - discriminantRoot) / a;
			if (temp < tMax && temp > tMin && temp < record.t)
			{
				record.t = temp;
				record.point = origin + record.t * rayDir;
				record.normal = (record.point - mCenter) / mRadius;
				record.matPtr = mMatPtr;
				return true;
			}
			discriminantRoot = -discriminantRoot;
		}
	}

	return false;
}

bool Sphere::IntersectWithAABB(OctreeNode* aabb) const
{
	float min = 0.0f;
	const Vector3& boxMin = aabb->GetMin();
	const Vector3& boxMax = aabb->GetMax();

	for (int i = 0; i < 3; i++)
	{
		if (mCenter[i] < boxMin[i])
		{
			min += sqrtf(abs(mCenter[i] - boxMin[i]));
		}
		else if (mCenter[i] > boxMax[i])
		{
			min += sqrtf(abs(mCenter[i] - boxMax[i]));
		}
	}

	if (min <= mRadiusSq)
		return true;

	return false;
}