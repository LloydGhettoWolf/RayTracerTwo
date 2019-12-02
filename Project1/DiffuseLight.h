#pragma once
#include "Materials.h"

class DiffuseLight : public Material
{
public:
	DiffuseLight(const Vector3& col) : mCol(col) {};
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const { return false; }
	virtual Vector3 Emitted(float u, float v, const Vector3& p) const { return mCol; }
private:
	Vector3 mCol;
};