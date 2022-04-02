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

	// ���� ����� �� ���� �ִٸ� 360���� ������ �������ش�.
	// �� �ڻ����� �������� ��� ��츦 ���Ѵ�.
	// ������ ��ǥ�迡���� y�� ���� �ݴ밡 �Ǿ��ִµ� ��, ����� ������ �� ���� ���� ��츦 ���Ѵ�.
	if (Dest.y < Src.y)
		Angle = 360.0f - Angle;

	return Angle;
}
