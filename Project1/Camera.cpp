#include "Camera.h"
#include "Globals.h"

Vector3 RandomInUnitDisc()
{
	Vector3 p;
	do {
		p = 2.0f * Vector3(RandFloat(), RandFloat(), 0.0f) - Vector3(1.0f, 1.0f, 0.0f);
	} while (p.DotProduct(p) >= 1.0f);
	return p;
}

Camera::Camera(const Vector3& eye, const Vector3& focus, const Vector3& up, float fov, float aspect, float aperture, float focusDist) : mOrigin(eye) {
	mLensRadius = aperture * 0.5f;
	float theta = fov * PI / 180.0f;
	float halfHeight = tanf(theta * 0.5f);
	float halfWidth = aspect * halfHeight;

	Vector3 lookAtVector = (focus - eye).GetNormalized();
	Vector3 rightVector = lookAtVector.CrossProduct(up).GetNormalized();
	Vector3 upVector = rightVector.CrossProduct(lookAtVector).GetNormalized();

	mLowerLeftCorner = mOrigin - (focusDist * rightVector * halfWidth) - (upVector * halfHeight * focusDist) + focusDist * lookAtVector;

	mHorizSpan = 2.0f * focusDist * rightVector * halfWidth;
	mVertSpan = 2.0f * focusDist * upVector * halfHeight;
};

Ray Camera::GetRay(float u, float v)
{

#ifdef GETSTATS
	IncreaseRays();
#endif

	Vector3 rd = mLensRadius * RandomInUnitDisc();
	Vector3 offset = u * rd[X] + v * rd[Y];
	return Ray(mOrigin + offset, mLowerLeftCorner + u * mHorizSpan + v * mVertSpan - mOrigin - offset);
}