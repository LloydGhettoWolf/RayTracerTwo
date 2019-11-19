#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray() {};
	Ray(const Vector3& origin, const Vector3& dir) : mOrigin(origin), mDir(dir.GetNormalized()) 
	{ 
		mInvDir = Vector3(1.0f) / mDir;
		mDot = mDir.DotProduct(mDir); 
	}

	inline const Vector3& GetOrigin() const { return mOrigin; }
	inline const Vector3& GetDir() const { return mDir; }
	inline const Vector3& GetInv() const { return mInvDir; }
	float   GetDot() const { return mDot; }

	Vector3 RayAtTimeT(float t) { return mOrigin + t * mDir; }
private:
	Vector3 mOrigin;
	Vector3 mDir;
	Vector3 mInvDir;
	float mDot;
};
