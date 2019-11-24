#include "Materials.h"

float Shlick(float cosine, float refIndex)
{
	float r0 = (1.0f - refIndex) / (1.0f + refIndex);
	r0 *= r0;
	return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
}