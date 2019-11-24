#include "Globals.h"

RayTraceStats stats;
std::mutex rayMutex;
std::mutex intMutex;

void IncreaseRays()
{
	std::lock_guard<std::mutex> myLock(rayMutex);
	stats.numRaysCreated++;
}

void IncreaseIntersections()
{
	std::lock_guard<std::mutex> myLock(intMutex);
	stats.numIntersectionsTests++;
}