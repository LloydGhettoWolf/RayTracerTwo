#pragma once

#include <math.h>
#include <random>
#include "Vector3.h"
#include "Ray.h"
#include "Primitive.h"


class Material
{
public:
	virtual bool Scatter(const Ray& in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;
	virtual Vector3 Emitted(float u, float v, const Vector3& p) const { return Vector3(0.0f); }
};


class Metal : public Material
{
public:
	Metal(const Vector3& a, float fuzz) : mAlbedo(a), mFuzz(fuzz) { if (mFuzz > 1.0f) mFuzz = 1.0f; }
	virtual bool Scatter(const Ray& in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
	{
		Vector3 reflected = Reflect(in.GetDir(), rec.normal);
		scattered = Ray(rec.point, reflected + mFuzz * RandinUnitSphere());
		attenuation = mAlbedo;
		return (scattered.GetDir().DotProduct(rec.normal) > 0);
	}
private:
	Vector3 mAlbedo;
	float mFuzz;
};


class Lambertian : public Material
{
public:
	Lambertian(const Vector3& a) : mAlbedo(a) {};
	virtual bool Scatter(const Ray& in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
	{
		Vector3 target = rec.point + rec.normal + RandinUnitSphere();
		scattered = Ray(rec.point, target - rec.point);
		attenuation = mAlbedo;
		return true;
	}
private:
	Vector3 mAlbedo;
};


float Shlick(float cosine, float refIndex);

class Dialectric : public Material
{
public:
	Dialectric(float refIndex) : mRefIndex(refIndex) {};
	virtual bool Scatter(const Ray& in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
	{
		Vector3 outwardNormal;
		Vector3 rayDir = in.GetDir();
		Vector3 reflected = Reflect(rayDir, rec.normal);

		float niOverNt;
		float reflectProb = 0.0f;
		float cosine = 0.0f;

		attenuation = Vector3(1.0f);
		Vector3 refracted;

		float dot = rayDir.DotProduct(rec.normal);

		if (dot > 0.0f)
		{
			outwardNormal = -rec.normal;
			niOverNt = mRefIndex;
			cosine = mRefIndex * dot / rayDir.GetLength();
		}
		else
		{
			outwardNormal = rec.normal;
			niOverNt = 1.0f / mRefIndex;
			cosine = -dot / rayDir.GetLength();
		}

		if (Refract(rayDir, outwardNormal, niOverNt, refracted))
		{
			//scattered = Ray(rec.point, refracted);
			reflectProb = Shlick(cosine, mRefIndex);
		}
		else
		{
			scattered = Ray(rec.point, reflected);
			reflectProb = 1.0f;
		}

		float randNorm = (float)(rand() % 40) / 40.0f;

		if (randNorm < reflectProb)
		{
			scattered = Ray(rec.point, reflected);
		}
		else
		{
			scattered = Ray(rec.point, refracted);
		}

		return true;
	}
private:
	float mRefIndex;
};

class Isotropic : public Material
{
	public:
		Isotropic(const Vector3& diff) : mDiffuse(diff) {};
		virtual bool Scatter(const Ray& r, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
		{
			scattered = Ray(rec.point, RandinUnitSphere());
			attenuation = mDiffuse;
			return true;
		}
	private:
		Vector3 mDiffuse;
};
