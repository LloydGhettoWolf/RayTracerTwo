#include "BVHNode.h"
#include <algorithm>
#include <iostream>
using namespace std;

BVHNode::BVHNode(Primitive** list, int n, float t0, float t1)
{
	int axis = rand() % 3;

	if (axis == 0)
	{
		qsort(list, n, sizeof(Primitive*), BoxXCompare);
	}
	else if (axis == 1)
	{
		qsort(list, n, sizeof(Primitive*), BoxYCompare);
	}
	else
	{
		qsort(list, n, sizeof(Primitive*), BoxZCompare);
	}

	if (n == 1)
	{
		mLeft = mRight = list[0];
	}
	else if (n == 2)
	{
		mLeft = list[0];
		mRight = list[1];
	}
	else
	{
		mLeft = new BVHNode(list, n / 2, t0, t1);
		mRight = new BVHNode(list + n / 2, n -  n / 2, t0, t1);
	}

	AABB boxLeft, boxRight;

	if (!mLeft->BoundingBox(t0, t1, boxLeft) || !mRight->BoundingBox(t0, t1, boxRight))
		cout << "no bounding box in bvh node constructor" << endl;

	mBox = SurroundingBox(boxLeft, boxRight);
}

bool BVHNode::Hit(const Ray& r, float tMin, float tMax, HitRecord& record, int depth) const
{
	if (mBox.Hit(r, tMin, tMax))
	{
		HitRecord leftRec, rightRec;

		bool hitLeft  = mLeft->Hit(r, tMin, tMax, leftRec, depth);
		bool hitRight = mRight->Hit(r, tMin, tMax, rightRec, depth);

		if (hitLeft && hitRight)
		{
			if (leftRec.t < rightRec.t)
			{
				record = leftRec;
			}
			else
			{
				record = rightRec;
			}

			return true;
		}
		else if (hitLeft)
		{
			record = leftRec;
			return true;
		}
		else if (hitRight)
		{
			record = rightRec;
			return true;
		}
		
		return false;
	}

	return false;
}

bool BVHNode::BoundingBox(float t0, float t1, AABB& box) const
{
	box = mBox;
	return true;
}