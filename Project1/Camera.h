#pragma once
#include "Vector3.h"
#include "Ray.h"
#define _USE_MATH_DEFINES
#include <cmath>

const float PI = 3.141592f;

Vector3 RandomInUnitDisc();

class Camera
{
public:
	Camera(const Vector3 &eye, const Vector3 &focus, const Vector3& up, float fov, float aspect, float aperture, float focusDist);

	Ray GetRay(float u, float v);
	
private:
	Vector3 mOrigin;
	Vector3 mLowerLeftCorner;
	Vector3 mHorizSpan;
	Vector3 mVertSpan;
	float mLensRadius;
};