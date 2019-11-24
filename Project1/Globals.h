#pragma once
// just have counters for rays created etc here - will have to be thread safe...
#include <mutex>

//#define GETSTATS

struct RayTraceStats
{
	int numRaysCreated = 0;
	int numIntersectionsTests = 0;
};

extern RayTraceStats stats;
extern std::mutex rayMutex;
extern std::mutex intMutex;

void IncreaseRays();
void IncreaseIntersections();