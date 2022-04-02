#pragma once

#include "EngineMath.h"

struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);
	Vector2(const XMVECTOR& v);
	
	// operator =
	Vector2& operator = (const Vector2& v);
	Vector2& operator = (const XMVECTOR& v);
	Vector2& operator = (float f);

	// operator ==, !=
	bool operator == (const Vector2& v)	const;
	bool operator == (const XMVECTOR& v)	const;
	bool operator != (const Vector2& v)	const;
	bool operator != (const XMVECTOR& v)	const;

	float& operator [] (int index);

	// operator +
	Vector2 operator + (const Vector2& v)	const;
	Vector2 operator + (const XMVECTOR& v)	const;
	Vector2 operator + (const float f)	const;

	// operator +=
	void operator += (const Vector2& v);
	void operator += (const XMVECTOR& v);
	void operator += (const float f);

	// operator -
	Vector2 operator - (const Vector2& v)	const;
	Vector2 operator - (const XMVECTOR& v)	const;
	Vector2 operator - (const float f)	const;

	// operator -=
	void operator -= (const Vector2& v);
	void operator -= (const XMVECTOR& v);
	void operator -= (const float f);

	// operator *
	Vector2 operator * (const Vector2& v)	const;
	Vector2 operator * (const XMVECTOR& v)	const;
	Vector2 operator * (const float f)	const;

	// operator *=
	void operator *= (const Vector2& v);
	void operator *= (const XMVECTOR& v);
	void operator *= (const float f);

	// operator /
	Vector2 operator / (const Vector2& v)	const;
	Vector2 operator / (const XMVECTOR& v)	const;
	Vector2 operator / (const float f)	const;

	// operator /=
	void operator /= (const Vector2& v);
	void operator /= (const XMVECTOR& v);
	void operator /= (const float f);

	// operator ++
	const Vector2& operator ++ ();
	const Vector2& operator ++ (int);

	// operator --
	Vector2 operator -- ();
	Vector2 operator -- (int);

	// 벡터의 길이를 구한다.
	float Length()	const;
	// v와의 거리를 구한다.
	float Distance(const Vector2& v)	const;
	// 벡터 정규화
	void Normalize();
	// 벡터 내적
	float Dot(const Vector2& v)	const;

	// Vector2 -> XMVECTOR로 변환
	XMVECTOR Convert()	const;
	// XMVECTOR -> Vector2로 변환
	void Convert(const XMVECTOR& v);
};

