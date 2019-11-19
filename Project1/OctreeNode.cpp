# include "OctreeNode.h"

OctreeNode::~OctreeNode()
{
	if(mChildren != nullptr)
		delete [] mChildren;
}

void OctreeNode::Insert(Primitive *node)
{
	if (node->IntersectWithAABB(this))
	{
		float size = abs(mMax[X] - mMin[X]);
		float halfSize = size * 0.5f;
		// is width of cube less than smallest?
		if (size <= mSmallest)
		{
			// then insert this primitive into the octree node
			mPrimitives.emplace_back(node);
			return;
		}
		else if(mNumChildren == 0)
		{
			// create lower layer of children
			mNumChildren = 8;
			// get center of this AABB
			Vector3 center = mMin + 0.5f * (mMax - mMin);
			
			//BOTTOM HALF

			// work out min and max co ordinates for new child
			// go in zig zag 
			// [ ][ ]
			// [g][ ]

			mChildren = new OctreeNode[8];

			mChildren[0] = OctreeNode(Vector3(mMin), Vector3(mMin[X] + halfSize, mMin[Y] + halfSize, mMin[Z] + halfSize), mSmallest);

			//[ ][ ]
			//[ ][g]

			mChildren[1] = OctreeNode(Vector3(mMin[X] + halfSize, mMin[Y], mMin[Z]), Vector3(mMax[X], mMin[Y] + halfSize, mMin[Z] + halfSize), mSmallest);

			//[g][ ]
			//[ ][ ]

			mChildren[2] = OctreeNode(Vector3(mMin[X], mMin[Y], mMin[Z] + halfSize), Vector3(mMin[X] + halfSize, mMin[Y] + halfSize, mMax[Z]), mSmallest);

			//[ ][g]
			//[ ][ ]

			mChildren[3] = OctreeNode(Vector3(mMin[X] + halfSize, mMin[Y], mMin[Z] + halfSize), Vector3(mMax[X], mMin[Y] + halfSize, mMax[Z]), mSmallest);

			//TOP HALF

			// work out min and max co ordinates for new child
			// go in zig zag 
			// [ ][ ]
			// [g][ ]

			mChildren[4] = OctreeNode(Vector3(mMin[X], mMin[Y] + halfSize, mMin[Z]), Vector3(mMin[X] + halfSize, mMax[Y], mMin[Z] + halfSize), mSmallest);

			//[ ][ ]
			//[ ][g]

			mChildren[5] = OctreeNode(Vector3(mMin[X] + halfSize, mMin[Y] + halfSize, mMin[Z]), Vector3(mMax[X], mMax[Y], mMin[Z] + halfSize), mSmallest);

			//[g][ ]
			//[ ][ ]

			mChildren[6] = OctreeNode(Vector3(mMin[X], mMin[Y] + halfSize, mMin[Z] + halfSize), Vector3(mMin[X] + halfSize, mMax[Y], mMax[Z]), mSmallest);

			//[ ][g]
			//[ ][ ]

			mChildren[7] = OctreeNode(Vector3(mMin[X] + halfSize, mMin[Y] + halfSize, mMin[Z] + halfSize), Vector3(mMax), mSmallest);
		}

		for (int child = 0; child < 8; child++)
			mChildren[child].Insert(node);
	}
}

bool OctreeNode::Hit(const Ray& r, float tMin, float tMax, HitRecord& record) const
{
	// early out
	if (mNumChildren == 0 && mPrimitives.size() == 0)
		return false;

	// do ray - box intersect
	Vector3 rayOrigin = r.GetOrigin();
	Vector3 invRayDir = r.GetInv();
	Vector3 t0 = (mMin - rayOrigin) * invRayDir;
	Vector3 t1 = (mMax - rayOrigin) * invRayDir;

	Vector3 tMinVec = Min(t0, t1);
	Vector3 tMaxVec = Max(t0, t1);
	 
	bool  intersection = MaxComponent(tMinVec) <= MinComponent(tMaxVec);

	bool hits = false;

	// if hit - pass to children if children exist
	if (intersection && (mNumChildren != 0))
	{
		for (int i = 0; i < mNumChildren; i++)
		{
			if (mChildren[i].Hit(r, tMin, tMax, record))
				hits = true;
		}

		return hits;
	}

	// otherwise looks like we need to run hit on all the primitives contained in here;
	for(auto itr = mPrimitives.begin(); itr != mPrimitives.end(); itr++)
	{
		if ((*itr)->Hit(r, tMin, tMax, record))
			hits = true;
	}

	return hits;
}