#include "Triangle.h"
#include "Globals.h"

bool Triangle::Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const
{
	// does the ray intersect with the plane that the triangle describes?
#ifdef GETSTATS
	IncreaseIntersections();
#endif

	float dotND = r.GetDir().DotProduct(mNormal);

	if (dotND > 0.0f)
		return false;

	// ok it hits, so does the intersect point lie inside of the triangle?
	float t = (md - mNormal.DotProduct(r.GetOrigin() - mVertices[0])) / dotND;

	if (t > record.t)
		return false;

	Vector3 intersectionPoint = r.GetOrigin() + t * r.GetDir();

	// test intersection point against 3 triangle lines to see if inside triangle

	for (int side = 0; side < 3; side++)
	{
		Vector3 s = mVertices[(side + 1) % 3] - mVertices[side];
		float value = (s).CrossProduct(intersectionPoint - mVertices[side]).DotProduct(mNormal);

		if (value < 0.0f)
			return false;
	}

	record.t = t;
	record.normal = mNormal;
	record.point = intersectionPoint;
	record.matPtr = mMatPtr;

	return true;
}