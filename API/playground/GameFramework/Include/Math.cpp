#include "Math.h"
#include "GameInfo.h"

Vector2::Vector2()	:
	x(0.f),
	y(0.f)
{
}

Vector2::Vector2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2& v)	:
	x(v.x),
	y(v.y)
{
}

float RadianToDegree(float Radian)
{
	return Radian * 180.0f / PI;
}

float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.0f;
}

float Distance(const Vector2& Src, const Vector2& Dest)
{
	Vector2 result = Src - Dest;

	return result.Length();
}

float GetAngleByAtan(const Vector2& Src, const Vector2& Dest)
{
	float x = Dest.x - Src.x;
	float y = Dest.y - Src.y;

	float Angle = RadianToDegree(atan2(y, x));

	return Angle;
}

float GetAngleByAcos(const Vector2& Src, const Vector2& Dest)
{
	float hyp = Distance(Src, Dest);
	float adj = Dest.x - Src.x;

	float Angle = adj / hyp;

	Angle = RadianToDegree(acosf(Angle));

	// 만약 대상이 더 위에 있다면 360에서 각도를 제거해준다.
	// 즉 코사인의 도메인을 벗어난 경우를 말한다.
	// 윈도우 좌표계에서는 y의 값이 반대가 되어있는데 즉, 대상이 나보다 더 위에 있을 경우를 말한다.
	if (Dest.y < Src.y)
		Angle = 360.0f - Angle;

	return Angle;
}
