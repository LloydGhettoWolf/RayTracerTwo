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


	bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth)  const override;
	Material* GetMaterial() const { return mMatPtr; }
private:
	Vector3 mVertices[3];
	Vector3 mSides[3];
	Vector3 mNormal;
	float md;
	Material *mMatPtr;
};