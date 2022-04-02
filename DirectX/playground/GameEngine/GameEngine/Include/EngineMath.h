#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <string.h>
#include <assert.h>

#include "Flag.h"

using namespace DirectX;

#define PI 3.14159f

static float DegreeToRadian(float angle)
{
	return angle / 180.0f * PI;
}

static float RadianToDegree(float angle)
{
	return angle * 180.0f / PI;
}

static float MapValue(float value, float low1, float high1, float low2, float high2)
{
	return low2 + (value - low1) * (high2 - low2) / (high1 / low1);
}