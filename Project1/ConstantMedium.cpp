#include "ConstantMedium.h"
#include "Vector3.h"

bool ConstantMedium::Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const
{
	bool db = false;
	HitRecord rec1, rec2;
	if (mBoundary->Hit(r, -tMax, tMax, rec1, depth))
	{
		if (mBoundary->Hit(r, rec1.t + 0.0001f, tMax, rec2, depth))
		{
			if (rec1.t < tMin) rec1.t = tMin;
			if (rec2.t > tMax) rec2.t = tMax;
			if (rec1.t >= rec2.t) return false;

			float distInside = (rec2.t - rec1.t) * r.GetDir().GetLength();
			float hitDistance = -(1.0f / mDensity) * log(RandFloat());

			if (hitDistance < distInside)
			{
				record.t = rec1.t + hitDistance / r.GetDir().GetLength();
				record.point = r.GetOrigin() + record.t * r.GetDir();
				record.normal = Vector3(1.0f, 0.0f, 0.0f);
				record.prim = (Primitive*)this;
				return true;
			}
		}
	}
	return false;
}

bool ConstantMedium::BoundingBox(float t0, float t1, AABB& box) const
{
	return mBoundary->BoundingBox(t0, t1, box);
}