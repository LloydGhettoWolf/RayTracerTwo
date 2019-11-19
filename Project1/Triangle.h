#pragma once
#include "Primitive.h"
#include "Vector3.h"
#include "Materials.h"

class Triangle : public Primitive
{
public:
	Triangle(Vector3 vertices[], Material* mat) : mVertices{ vertices[0], vertices[1], vertices[2] }, mMatPtr(mat)
	{
		//calculate normal
		Vector3 AB = mVertices[1] - mVertices[0];
		Vector3 AC = mVertices[2] - mVertices[0];

		mNormal = AB.CrossProduct(AC).GetNormalized();

		md = mNormal.DotProduct(mVertices[0]);

		mSides[0] = mVertices[1] - mVertices[0];
		mSides[1] = mVertices[2] - mVertices[1];
		mSides[2] = mVertices[0] - mVertices[2];
	};

	~Triangle()
	{
		if (mMatPtr != nullptr)
			delete mMatPtr;
	}

	bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const
	{
		// does the ray intersect with the plane that the triangle describes?
		
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

	virtual bool IntersectWithAABB(OctreeNode* aabb) const { return true; }

private:
	Vector3 mVertices[3];
	Vector3 mSides[3];
	Vector3 mNormal;
	float md;
	Material *mMatPtr;
};