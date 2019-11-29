#pragma once

#include <math.h>
#include <random>

const static int X = 0;
const static int Y = 1;
const static int Z = 2;

class Vector3
{
public:
	Vector3() : values{0.0f, 0.0f, 0.0f} {};
	Vector3(float value) : values{ value, value, value } {};
	Vector3(float x, float y, float z) : values{ x, y, z} {};

	inline Vector3 operator+(const Vector3& rhs) const;
	inline Vector3 operator-(const Vector3& rhs) const;
	inline Vector3 operator*(float multiplier)   const;
	inline Vector3 operator*(const Vector3& rhs) const;
	inline Vector3 operator/(const Vector3& rhs) const;
	inline Vector3 operator/(float divisor) const;
	
	inline Vector3& operator+=(const Vector3& rhs);
	inline Vector3& operator-=(const Vector3& rhs);
	inline Vector3& operator*=(float multiplier);
	
	inline Vector3 operator-() const;

	inline const float& operator[](int index) const;
	inline float& operator[](int index);

	inline float   GetLength()const;
	inline Vector3 GetNormalized()const;
	inline float   DotProduct(const Vector3& other)const;
	inline Vector3 CrossProduct(const Vector3& other)const;

private:
	union
	{
		struct {
			float mX, mY, mZ;
		};
		float values[3];
	};
};

inline Vector3 operator*(float multiplier, const Vector3& rhs);

inline Vector3 RandinUnitSphere();
inline Vector3 Reflect(const Vector3& vector, const Vector3& normal);
inline bool Refract(const Vector3& vector, const Vector3& normal, float niOverNt, Vector3& refracted);
inline float RandFloat();

inline Vector3 Max(const Vector3& first, const Vector3& second);
inline Vector3 Min(const Vector3& first, const Vector3& second);

inline float MinComponent(const Vector3& vec);
inline float MaxComponent(const Vector3& vec);


static float randValues[256];

Vector3 Vector3::operator+(const Vector3& rhs) const
{
	return Vector3(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ);
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	return Vector3(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ);
}

Vector3 Vector3::operator*(float multiplier) const
{
	return Vector3(multiplier * mX, multiplier * mY, multiplier * mZ);
}

Vector3 Vector3::operator*(const Vector3& rhs) const
{
	return Vector3(mX * rhs[X], mY * rhs[Y], mZ * rhs[Z]);
}

Vector3 Vector3::operator/(float divisor) const
{
	float multiplier = 1.0f / divisor;
	return Vector3(multiplier * mX, multiplier * mY, multiplier * mZ);
}

Vector3 Vector3::operator/(const Vector3& rhs) const
{
	return Vector3(mX / rhs[X], mY / rhs[Y], mZ / rhs[Z]);
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
	mX += rhs.mX;
	mY += rhs.mY;
	mZ += rhs.mZ;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	mX -= rhs.mX;
	mY -= rhs.mY;
	mZ -= rhs.mZ;
	return *this;
}

Vector3& Vector3::operator*=(float multiplier)
{
	mX *= multiplier;
	mY *= multiplier;
	mZ *= multiplier;
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-mX, -mY, -mZ);
}

Vector3 operator*(float multiplier, const Vector3& rhs)
{
	return Vector3(multiplier * rhs[X], multiplier * rhs[Y], multiplier * rhs[Z]);
}

const float& Vector3::operator[](int index) const
{
	return values[index];
}

float& Vector3::operator[](int index)
{
	return values[index];
}

float   Vector3::DotProduct(const Vector3& other)const
{
	float a = mX * other.mX;
	float b = mY * other.mY;
	float c = mZ * other.mZ;
	return a + b + c;
}

Vector3   Vector3::CrossProduct(const Vector3& other)const
{
	float newX = mY * other[Z] - mZ * other[Y];
	float newY = mX * other[Z] - mZ * other[X];
	float newZ = mX * other[Y] - mY * other[X];
	return Vector3(newX, -newY, newZ);
}

float   Vector3::GetLength()const
{
	float xLen = mX * mX;
	float yLen = mY * mY;
	float zLen = mZ * mZ;

	return sqrtf(xLen + yLen + zLen);
}

Vector3 Vector3::GetNormalized()const
{
	float len = GetLength();
	return Vector3(*this) / len;
}



Vector3 RandinUnitSphere()
{
	float randX = RandFloat();
	float randY = RandFloat();
	float randZ = RandFloat();

	Vector3 rand(randX, randY, randZ);
	return rand * 2.0f - Vector3(1.0f, 1.0f, 1.0f);
}

Vector3 Reflect(const Vector3& vector, const Vector3& normal)
{
	return vector - 2.0f * vector.DotProduct(normal) * normal;
}

bool Refract(const Vector3& vector, const Vector3& normal, float niOverNt, Vector3& refracted)
{
	Vector3 uv = vector.GetNormalized();
	float dt = uv.DotProduct(normal);
	float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
	if (discriminant > 0.0f)
	{
		refracted = niOverNt * (uv - dt * normal) - sqrtf(discriminant) * normal;
		return true;
	}
	return false;
}

Vector3 Min(const Vector3& first, const Vector3& second)
{
	float minX = first[X] < second[X] ? first[X] : second[X];
	float minY = first[Y] < second[Y] ? first[Y] : second[Y];
	float minZ = first[Z] < second[Z] ? first[Z] : second[Z];

	return Vector3(minX, minY, minZ);
}

Vector3 Max(const Vector3& first, const Vector3& second)
{
	float maxX = first[X] > second[X] ? first[X] : second[X];
	float maxY = first[Y] > second[Y] ? first[Y] : second[Y];
	float maxZ = first[Z] > second[Z] ? first[Z] : second[Z];

	return Vector3(maxX, maxY, maxZ);
}

float MinComponent(const Vector3& vec)
{
	return vec[X] < vec[Y] ? (vec[X] < vec[Z] ? vec[X] : vec[Z]) : (vec[Y] < vec[Z] ? vec[Y] : vec[Z]);
}

float MaxComponent(const Vector3& vec)
{
	return vec[X] > vec[Y] ? (vec[X] > vec[Z] ? vec[X] : vec[Z]) : (vec[Y] > vec[Z] ? vec[Y] : vec[Z]);
}



float RandFloat()
{
	static float multiplier = 1.0f / (float)RAND_MAX;
	return (float)rand() * multiplier; ;
}