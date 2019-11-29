#pragma once
#include <mutex>
#include <queue>

class Camera;
class Vector3;
class Primitive;
class PrimitiveList;

struct RayArgs
{
	float x;
	float y;
	Camera* cam;
	Vector3* samplePositions;
	PrimitiveList** primList;
	float sampleMultiplier;
	unsigned char* dataPtr;
};

typedef  void* (*vdFunc)(void*);

struct workItem
{
	vdFunc func;
	RayArgs args;
};

extern std::mutex queueMutex;
extern std::queue<workItem> workQueue;
extern std::condition_variable cond;

extern bool notFinished;

void WorkerThread();