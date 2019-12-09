

#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <string>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>

#include "Ray.h"
#include "Primitive.h"
#include "PrimitiveList.h"
#include "Materials.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "Globals.h"
#include "WorkerThreads.h"
#include "BVHNode.h"
#include "Rectangle.h"
#include "DiffuseLight.h"
#include "Box.h"

using namespace std;

const int WIDTH  = 640;
const int HEIGHT = 480;

const float vertIncr = 1.0f / float(HEIGHT);
const float horizIncr = 1.0f / float(WIDTH);

const float ASPECT = (float)WIDTH / (float)HEIGHT;
const int NUM_COLS_PER_PIXEL = 3;

const int SPAN_LENGTH = WIDTH * NUM_COLS_PER_PIXEL;

const int NUM_SAMPLES = 16;
const int DEPTH = 25;

const int NUM_SPHERES = 8;

const static string fileName = "test.tga";

// will we run on multicore?
#define MULTICORE


void OutputPPM(int width, int height, char* data)
{
	FILE* out;
	out = fopen(fileName.c_str(), "wb");

	char header[18] = { 0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	header[12] = width & 0xFF;
	header[13] = (width >> 8) & 0xFF;
	header[14] = (height) & 0xFF;
	header[15] = (height >> 8) & 0xFF;
	header[16] = 24;

	fwrite(header, sizeof(char), 18, out);
	int numBytes = width * height * NUM_COLS_PER_PIXEL;
	fwrite(data, sizeof(char), numBytes, out);
	fclose(out);
}


Primitive* CreateScene()
{
	int n = 500;
	Primitive** list = new Primitive* [n + 1];

	//Vector3 triVerts[3];

	//triVerts[0] = Vector3(-2.0f, 0.0f, 0.0f);
	//triVerts[1] = Vector3(2.0f, 0.0f, 0.0f);
	//triVerts[2] = Vector3(2.0f, 2.0f, 0.0f);

	int i = 0;

	////list[i++] = new Triangle( triVerts, new Metal(Vector3(0.5f, 0.5f, 0.5f), 0.05f));
	list[i++] = new Sphere(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(Vector3(0.5f, 0.5f, 0.5f)));
	
	for (int a = -NUM_SPHERES; a < NUM_SPHERES; a++)
	{
		for (int b = -NUM_SPHERES; b < NUM_SPHERES; b++)
		{
			float chooseMat = RandFloat();
			Vector3 center((float)a + (2.0f * RandFloat() -1.0f), 0.225f, b + (2.0f * RandFloat() - 1.0f));
			if (chooseMat < 0.8f)
			{
				list[i++] = new Sphere(center, 0.25f, new Lambertian(Vector3(RandFloat(), RandFloat(), RandFloat())));
			}
			else if (chooseMat < 0.95f)
			{
				list[i++] = new  Sphere(center, 0.25f, new Metal(Vector3(RandFloat(), RandFloat(), RandFloat()), 0.3f * RandFloat()));
			}
			else
			{
				list[i++] = new Sphere(center, 0.25f, new Dialectric(1.5f));
			}
		}
	}

	list[i++] = new Sphere(Vector3(0.0f, 1.0f, 0.0f), 1.0f, new Dialectric(1.5f));
	list[i++] = new Sphere(Vector3(0.0f, 1.0f, 0.0f), -0.95f, new Dialectric(1.5f));
	list[i++] = new Sphere(Vector3(2.0f, 1.0f, 0.0f), 1.0f, new Metal(Vector3(0.7f, 0.6f, 0.5f), 0.0f));

	//list[i++] = new Rectangle(3.0f, 5.0f, 1.0f, 3.0f,    -2.0f,    new DiffuseLight(Vector3(4.0f)), RECT_TYPE::XY);

	//list[i++] = new Box(Vector3(-0.5f, 0.0f, 1.0f), Vector3(0.5f, 1.0f, 0.0f), new Metal(Vector3(0.7f, 0.6f, 0.5f), 0.0f));
	list[i++] = new Sphere(Vector3(0.0f, 4.0f, 0.0f), 1.0f, new DiffuseLight(Vector3(4.0f)));
	return new BVHNode(list, i, 0.0f, 1.0f);
}


float RandOffset(float var) 
{
	float rand = RandFloat() * var;
	return (rand * 2.0f) - var;
}



Vector3 BGColor(const Ray& r, Primitive* list, int depth)
{
	HitRecord rec;

	if (list->Hit(r, 0.01f, 9999.9f, rec, depth))
	{
		rec.matPtr = rec.prim->GetMaterial();
		rec.point = r.GetOrigin() + rec.t * r.GetDir();
		rec.normal = rec.prim->GetNormal(rec.point);
		Ray scattered;
		Vector3 attenuation;
		Vector3 emittedLight = rec.matPtr->Emitted(1.0f, 1.0f, Vector3(1.0f));
		if (depth < DEPTH && rec.matPtr->Scatter(r, rec, attenuation, scattered))
		{
			return emittedLight + attenuation * BGColor(scattered, list, depth + 1);
		}
		else
		{
			return emittedLight;
		}
	
	}

	return Vector3(0.0f);
}

#ifdef MULTICORE

void* TraceRay(void* arg)
{
	RayArgs* args = (RayArgs*)arg;

	Vector3 result(0.0f);

	for (int sample = 0; sample < NUM_SAMPLES; sample++)
	{
		float u = args->x + RandOffset(horizIncr);
		float v = args->y + RandOffset(vertIncr);
		Ray r = args->cam->GetRay(u, v);
		result += BGColor(r, args->primList, 1);
	}

	result *= args->sampleMultiplier;

	unsigned char vals[3] = { unsigned char(sqrtf(result[Z]) * 255.99f),
							  unsigned char(sqrtf(result[Y]) * 255.99f),
							  unsigned char(sqrtf(result[X]) * 255.99f) };

	memcpy((void*)args->dataPtr, (void*)vals, 3);

	return nullptr;
}

void TraceRays(int startY, int finishY, int startX, int finishX, float horizIncr, float vertIncr, Camera& camera, Primitive* primList, float sampleMultiplier,
	unsigned char* dataPtr)
{

	unsigned char* dataStart = dataPtr;

	for (int i = startY; i < finishY; i++) {

		

		for (int j = startX; j < finishX; j++)
		{
			//Vector3 result;
			float u = (float)j * horizIncr;
			float v = (float)i * vertIncr;

			RayArgs args;
			args.cam = &camera;
			args.x = u;
			args.y = v;
			args.primList = primList;
			args.sampleMultiplier = sampleMultiplier;
			args.dataPtr = dataStart;
			
			workItem item;
			item.func = TraceRay;
			item.args = args;

			lock_guard<mutex> lk(queueMutex);
				workQueue.push(item);
				cond.notify_one();

			dataStart += 3;
		}
	}
}
#else
void TraceRay(float x, float y, Camera& camera, Vector3* samplePositions, PrimitiveList* primList, float sampleMultiplier, unsigned char* dataPtr)
{
	Vector3 result(0.0f);

	for (int sample = 0; sample < NUM_SAMPLES; sample++)
	{
		float u = x + samplePositions[sample][X];
		float v = y + samplePositions[sample][Y];
		Ray r = camera.GetRay(u,v);
		result += BGColor(r, primList, 1);
	}

	result *= sampleMultiplier;

	unsigned char vals[3] = { unsigned char(sqrtf(result[Z]) * 255.99f),
							  unsigned char(sqrtf(result[Y]) * 255.99f),
							  unsigned char(sqrtf(result[X]) * 255.99f) };

	memcpy((void*)dataPtr, (void*)vals, 3);
}

void TraceRays(int startY, int finishY, int startX, int finishX, float horizIncr, float vertIncr, Camera& camera, Vector3* samplePositions, PrimitiveList* primList, float sampleMultiplier,
	unsigned char* dataPtr)
{
	for (int i = 0; i < HEIGHT; i++) {

		unsigned char* dataStart = dataPtr + i * SPAN_LENGTH;

		for (int j = 0; j < WIDTH; j++)
		{
			Vector3 result;
			float u = (float)j * horizIncr;
			float v = (float)i * vertIncr;

			TraceRay(u, v, camera, samplePositions, primList, sampleMultiplier, dataStart);
			dataStart += 3;
		}
	}
}
#endif 


int main() {

	srand(time(0));
	size_t dataSz = WIDTH * HEIGHT * NUM_COLS_PER_PIXEL;
	unsigned char* data = new unsigned char[dataSz];
	unsigned char* dataPtr = data;

	float x = 0.0f, y = 0.0f;

	float aspect = 1.0f / ASPECT;

	float sampleMultiplier = 1.0f / (float)NUM_SAMPLES;

	float R = PI * 0.25f;
	Vector3 eye(-6.0f, 5.0f, 20.0f);
	Vector3 focus(0.0f, 0.5f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);

	Camera camera(eye, focus, up, 20.0f, ASPECT, 0.1f, 20.0f);

	auto startTime = chrono::high_resolution_clock::now();
		Primitive* primList = CreateScene();
	auto endTime = chrono::high_resolution_clock::now();

	chrono::duration<double> execTime = endTime - startTime;

	cout << "Time taken to build bvh : " << execTime.count() << " seconds" << endl;

#ifndef MULTICORE
	startTime = chrono::high_resolution_clock::now();
	TraceRays(0, HEIGHT, 0, WIDTH, horizIncr, vertIncr, camera, samplePositions, primList, sampleMultiplier, dataPtr);
	endTime = chrono::high_resolution_clock::now();

	execTime = endTime - startTime;

	cout << "Time taken : " << execTime.count() << " seconds" << endl;
#else

	int numCores = thread::hardware_concurrency();

	thread *traceThreads = new thread[numCores-1];

	int yIncr = HEIGHT / numCores;
	int dataStride = WIDTH * yIncr * 3;

	for (int i = 0; i < (numCores - 1); i++)
		traceThreads[i] = thread(WorkerThread);

	startTime = chrono::high_resolution_clock::now();


	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++)
		{
			//Vector3 result;
			float u = (float)j * horizIncr;
			float v = (float)i * vertIncr;

			RayArgs args;
			args.cam = &camera;
			args.x = u;
			args.y = v;
			args.primList = primList;
			args.sampleMultiplier = sampleMultiplier;
			args.dataPtr = dataPtr;

			workItem item;
			item.func = TraceRay;
			item.args = args;

			lock_guard<mutex> lk(queueMutex);
			workQueue.push(item);
			cond.notify_one();

			dataPtr += 3;
		}
	}
	
	while (!workQueue.empty()) cond.notify_one();

	notFinished = false;

	RayArgs args;
	args.cam = &camera;
	args.x = 0;
	args.y = 0;
	args.primList = primList;
	args.sampleMultiplier = sampleMultiplier;
	args.dataPtr = 0;

	workItem item;
	item.func = TraceRay;
	item.args = args;

	workQueue.push(item);
	cond.notify_all();

	for (int i = 0; i < (numCores - 1); i++)
	{
		traceThreads[i].join();
	}

	endTime = chrono::high_resolution_clock::now();

	execTime = endTime - startTime;

	cout << "Time taken : " << execTime.count() << " seconds" << endl;


	delete[] traceThreads;
#endif

	OutputPPM(WIDTH, HEIGHT, (char*)data);
	
	delete[] data;

	
#ifdef GETSTATS
	cout << " num rays created : " << stats.numRaysCreated << endl;
	cout << " num intersection tests : " << stats.numIntersectionsTests << endl;
#endif

	return 0;
}