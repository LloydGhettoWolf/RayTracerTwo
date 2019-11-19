//#define SIMD 1;

#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <string>
#include <thread>
#include "OctreeNode.h"

#ifdef SIMD
#else
#include "Vector3.h"
#endif


#include "Ray.h"
#include "Primitive.h"
#include "PrimitiveList.h"
#include "Materials.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
using namespace std;

const int WIDTH  = 1280;
const int HEIGHT = 1080;
const float ASPECT = (float)WIDTH / (float)HEIGHT;
const int NUM_COLS_PER_PIXEL = 3;

const int SPAN_LENGTH = WIDTH * NUM_COLS_PER_PIXEL;

const int NUM_SAMPLES = 4;

const int NUM_SPHERES = 6;

const int NODE_DIMENSION = 16;

const static string fileName = "test.tga";

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
	size_t numBytes = width * height * NUM_COLS_PER_PIXEL;
	fwrite(data, sizeof(char), numBytes, out);
	fclose(out);
}

OctreeNode* CreateScene(int &numPrims)
{
	int n = 500;
	Primitive** list = new Primitive* [n + 1];

	Vector3 triVerts[3];

	triVerts[0] = Vector3(-1.0f, 0.0f, 0.0f);
	triVerts[1] = Vector3(1.0f, 0.0f, 0.0f);
	triVerts[2] = Vector3(0.0f, 1.0f, 0.0f);

	//list[0] = new Triangle( triVerts, new Lambertian(Vector3(0.5f, 0.5f, 0.5f)));

	list[0] = new Sphere(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(Vector3(0.5f, 0.5f, 0.5f)));

	int i = 1;

	size_t sphereSz = sizeof(Sphere);

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
				list[i++] = new  Sphere(center, 0.25f, new Metal(Vector3(RandFloat(), RandFloat(), RandFloat()), 0.5f * RandFloat()));
			}
			else
			{
				list[i++] = new Sphere(center, 0.25f, new Dialectric(1.5f));
			}
		}
	}

	
	list[i++] = new Sphere(Vector3(0.0f, 1.0f, 0.0f), 1.0f, new Dialectric(1.5f));

	list[i++] = new Sphere(Vector3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vector3(0.7f, 0.6f, 0.5f), 0.0f));

	numPrims = i;

	//// try inserting the lsit of spheres into a quadtree

	OctreeNode* ptr = new OctreeNode(Vector3(-NODE_DIMENSION, 0.0f, -NODE_DIMENSION), Vector3(NODE_DIMENSION, 2.0f, NODE_DIMENSION), 1.0f);

	for (int sphere = 0; sphere < i; sphere++)
	{
		ptr->Insert(list[sphere]);
	}

	return ptr;

	//return new PrimitiveList(list, i);
}


Vector3 RandOffset(float xVar, float yVar) 
{
	float randX = RandFloat();
	float randY = RandFloat();

	xVar = -(xVar * 0.5f) +  xVar * randX;
	yVar = -(yVar * 0.5f) +  yVar * randY;

	return Vector3(xVar, yVar, 0.0f);
}



Vector3 BGColor(const Ray& r, OctreeNode* list, int depth)
{
	HitRecord rec;

	if (list->Hit(r, 0.01f, 9999.9f, rec))
	{
		Ray scattered;
		Vector3 attenuation;

		if (depth < 10 && rec.matPtr->Scatter(r, rec, attenuation, scattered))
		{
			return attenuation * BGColor(scattered, list, depth + 1);
		}
		else
		{
			return Vector3(0.0f);
		}
	}

	Vector3 unitDir = r.GetDir();
	float t = 0.5f * (unitDir[Y] + 1.0f);
	return (1.0f - t) * Vector3(1.0f) + t * Vector3(0.5f, 0.7f, 1.0f);
}


void TraceRays(int startY, int finishY, int startX, int finishX, float horizIncr, float vertIncr, Camera& camera, Vector3 *samplePositions, OctreeNode* primList, float sampleMultiplier,
				unsigned char *dataPtr)
{

	for (int i = startY; i < finishY; i++) {

		unsigned char* dataStart = dataPtr + i * SPAN_LENGTH;

		for (int j = startX; j < finishX; j++)
		{
			Vector3 result;
			float u = (float)j * horizIncr;
			float v = (float)i * vertIncr;
			if (NUM_SAMPLES == 1)
			{
				Ray r = camera.GetRay(u, v);
				result = BGColor(r, primList, 1);
			}
			else
			{
				for (int sample = 0; sample < NUM_SAMPLES; sample++)
				{

					Ray r = camera.GetRay(u + samplePositions[sample][X], v + samplePositions[sample][Y]);
					result += BGColor(r, primList, 1);
				}
			}


			result *= sampleMultiplier;

			*dataStart = unsigned char(sqrtf(result[Z]) * 255.99f);
			dataStart++;

			*dataStart = unsigned char(sqrtf(result[Y]) * 255.99f);
			dataStart++;

			*dataStart = unsigned char(sqrtf(result[X]) * 255.99f);
			dataStart++;
		}
	}
}

int main() {


	srand(time(0));
	size_t dataSz = WIDTH * HEIGHT * NUM_COLS_PER_PIXEL;
	unsigned char* data = new unsigned char[dataSz];
	unsigned char* dataPtr = data;

	float x = 0.0f, y = 0.0f;

	float aspect = 1.0 / ASPECT;
	float vertIncr = 1.0f / float(HEIGHT);
	float horizIncr = 1.0f / float(WIDTH);

	Vector3 samplePositions[NUM_SAMPLES];
	float sampleMultiplier = 1.0f / (float)NUM_SAMPLES;

	for (int sample = 0; sample < NUM_SAMPLES; sample++)
		samplePositions[sample] = RandOffset(horizIncr, vertIncr);


	float R = PI * 0.25f;
	Vector3 eye(3.0f, 3.0f, 10.0f);
	Vector3 focus(0.0f, 0.5f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);

	Camera camera(eye, focus, up, 20.0f, ASPECT, 0.1f, 10.0f);

	int numPrims;
	OctreeNode* primList = CreateScene(numPrims);
	//PrimitiveList *primList  = CreateScene(numPrims);


	//TraceRays(0, HEIGHT, 0, WIDTH, horizIncr, vertIncr, camera, samplePositions, primList, sampleMultiplier, dataPtr);

	thread traceThreads[16];

	int xIncr = WIDTH / 8;
	int dataStride = xIncr * 3;

	for (int i = 0; i < 8; i++)
	{
		int xEnd = xIncr * (i + 1);
		dataPtr = data + i * dataStride;
		traceThreads[i] = thread(TraceRays, 0, HEIGHT / 2, xIncr * i, xEnd, horizIncr, vertIncr, std::ref(camera), samplePositions, primList, sampleMultiplier, dataPtr);
	}

	for (int i = 8; i < 16; i++)
	{
		int xStart = xIncr * (i - 8);
		int xEnd = xIncr * (i - 7);
		dataPtr = data + i * dataStride;
		traceThreads[i] = thread(TraceRays, HEIGHT / 2 - 1, HEIGHT, xStart, xEnd, horizIncr, vertIncr, std::ref(camera), samplePositions, primList, sampleMultiplier, dataPtr);
	}

	for (int i = 0; i < 16; i++)
	{
		traceThreads[i].join();
	}

	
	OutputPPM(WIDTH, HEIGHT, (char*)data);
	
	/*Primitive** primitives = primList->GetList();
	delete[] primitives;

	delete[] primList;*/

	return 0;
}