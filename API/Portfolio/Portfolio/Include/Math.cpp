#include "Math.h"

Vector2::Vector2() :
	x(0.0f),
	y(0.0f)
{
}

Vector2::Vector2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2& v) :
	x(v.x),
	y(v.y)
{
}

float Rad2Deg(float radian)
{
	return radian * 180.0f / M_PI;
}

float Deg2Rad(float degree)
{
	return degree * M_PI / 180.0f;
}

float Distance(const Vector2& src, const Vector2& dest)
{
	Vector2 result = src - dest;

	return result.Length();
}

float GetAngle(const Vector2& src, const Vector2& dest)
{
	float x = dest.x - src.x;
	float y = dest.y - src.y;

	float angle = Rad2Deg(atan2(y, x));

	return angle;
}
