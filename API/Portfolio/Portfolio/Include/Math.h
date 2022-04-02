#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>


struct Vector2
{
	float	x;
	float	y;

public:
	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);

public:
	const Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	const Vector2& operator = (const POINT& v)
	{
		x = (float)v.x;
		y = (float)v.y;

		return *this;
	}

	// +
	Vector2 operator + (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vector2 operator + (const POINT& v)	const
	{
		Vector2	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vector2 operator + (float f)	const
	{
		Vector2	result;
		result.x = x + f;
		result.y = y + f;
		return result;
	}

	Vector2 operator + (int i)	const
	{
		Vector2	result;
		result.x = x + i;
		result.y = y + i;
		return result;
	}

	// +=
	const Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const Vector2& operator += (const POINT& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const Vector2& operator += (float f)
	{
		x += f;
		y += f;
		return *this;
	}

	const Vector2& operator += (int i)
	{
		x += i;
		y += i;
		return *this;
	}

	// -
	Vector2 operator - (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vector2 operator - (const POINT& v)	const
	{
		Vector2	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vector2 operator - (float f)	const
	{
		Vector2	result;
		result.x = x - f;
		result.y = y - f;
		return result;
	}

	Vector2 operator - (int i)	const
	{
		Vector2	result;
		result.x = x - i;
		result.y = y - i;
		return result;
	}

	// -=
	const Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const Vector2& operator -= (const POINT& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const Vector2& operator -= (float f)
	{
		x -= f;
		y -= f;
		return *this;
	}

	const Vector2& operator -= (int i)
	{
		x -= i;
		y -= i;
		return *this;
	}

	// *
	Vector2 operator * (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	Vector2 operator * (const POINT& v)	const
	{
		Vector2	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	Vector2 operator * (float f)	const
	{
		Vector2	result;
		result.x = x * f;
		result.y = y * f;
		return result;
	}

	Vector2 operator * (int i)	const
	{
		Vector2	result;
		result.x = x * i;
		result.y = y * i;
		return result;
	}

	// *=
	const Vector2& operator *= (const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const Vector2& operator *= (const POINT& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const Vector2& operator *= (float f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	const Vector2& operator *= (int i)
	{
		x *= i;
		y *= i;
		return *this;
	}

	// /
	Vector2 operator / (const Vector2& v)	const
	{
		Vector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	Vector2 operator / (const POINT& v)	const
	{
		Vector2	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	Vector2 operator / (float f)	const
	{
		Vector2	result;
		result.x = x / f;
		result.y = y / f;
		return result;
	}

	Vector2 operator / (int i)	const
	{
		Vector2	result;
		result.x = x / i;
		result.y = y / i;
		return result;
	}

	// /=
	const Vector2& operator /= (const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector2& operator /= (const POINT& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const Vector2& operator /= (float f)
	{
		x /= f;
		y /= f;
		return *this;
	}

	const Vector2& operator /= (int i)
	{
		x /= i;
		y /= i;
		return *this;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y);
	}

	// 길이를 1로 만들어준다.
	void Normalize()
	{
		float	SelfLength = Length();

		if (SelfLength <= 0.0f)
			return;

		x /= SelfLength;
		y /= SelfLength;
	}
};

float Rad2Deg(float radian);
float Deg2Rad(float degree);
float Distance(const Vector2& src, const Vector2& dest);
float GetAngle(const Vector2& src, const Vector2& dest);