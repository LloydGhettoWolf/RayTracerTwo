#pragma once
#include "Primitive.h"
#include "Vector3.h"
#include "NodeType.h"
#include <vector>

class OctreeNode : public NodeType
{
public:
	OctreeNode() {};
	OctreeNode(const Vector3& min, const Vector3& max, float smallest) : mMin(min), mMax(max), mNumChildren(0), mSmallest(smallest), mChildren(nullptr) {};
	~OctreeNode();

	void Insert(Primitive *node);

	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const;


	const Vector3& GetMin() const { return mMin; }
	const Vector3& GetMax() const { return mMax; }
private:
	Vector3 mMin, mMax;
	int mNumChildren;
	float mSmallest;
	OctreeNode *mChildren;
	std::vector<Primitive*> mPrimitives;
};