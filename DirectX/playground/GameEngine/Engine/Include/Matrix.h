#pragma once

#include "Vector4.h"

// 행렬의 경우 빠른 연산을위해 벡터 레지스터를 이용한다.
// 이 경우 16byte로 정렬된 데이터를 만든다.
// 또한 공용체를 이용해서 쉽게 각각 요소에 접근 할 수 있게한다.
union __declspec(align(16)) Matrix
{
	XMMATRIX	m;
	
	// XMATRIX에는 총 float타입 변수 16개가 들어있다.
	// 따라서 아래의 변수들로 공용체의 특성을 이용해서 쉽게 접근이 가능하다.
	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};


	// 또한 Vector4타입도 존재하여
	// v[0].x, v[0].y, v[0].z, v[0].w 이런식으로도 접근이 가능
	Vector4	v[4];

	Matrix();
	Matrix(const Matrix& _m);
	Matrix(const XMMATRIX& _m);
	Matrix(const Vector4 _v[4]);

	Vector4& operator [] (int index);
	Matrix operator = (const Matrix& _m);
	Matrix operator = (const XMMATRIX& _m);
	Matrix operator = (const Vector4 _v[4]);

	Matrix operator * (const Matrix& _m)	const;
	Matrix operator * (const XMMATRIX& _m)	const;

	const Matrix& operator *= (const Matrix& _m);
	const Matrix& operator *= (const XMMATRIX& _m);

	// 단위행렬로 만든다.
	void Identity();
	// 전치행렬로 만든다.
	void Transpose();
	// 역행렬로 만든다.
	void Inverse();
	// x, y, z 스케일링
	void Scaling(const struct Vector3& _v);
	void Scaling(float x, float y, float z);

	// x, y 스케일링
	void Scaling(const struct Vector2& _v);
	void Scaling(float x, float y);

	// x, y, z 축 회전
	void Rotation(const struct Vector3& _v);
	void Rotation(float x, float y, float z);

	// x 축회전
	void RotationX(float x);

	// y축 회전
	void RotationY(float y);

	// z축 회전
	void RotationZ(float z);

	// 쿼터니온을 이용한 회전
	void RotationQuaternion(const Vector4& qut);

	// x, y, z 축이아닌 임의의 축으로 회전한다.
	void RotationAxis(const struct Vector3& Axis, float Angle);

	// x, y, z 이동
	void Translation(const struct Vector3& _v);
	void Translation(float x, float y, float z);

	// x, y 이동
	void Translation(const struct Vector2& _v);
	void Translation(float x, float y);

	static Matrix StaticIdentity();
	static Matrix StaticTranspose(const Matrix& _m);
	static Matrix StaticInverse(const Matrix& _m);
	static Matrix StaticScaling(const struct Vector3& _v);
	static Matrix StaticScaling(float x, float y, float z);
	static Matrix StaticScaling(const struct Vector2& _v);
	static Matrix StaticScaling(float x, float y);
	static Matrix StaticRotation(const struct Vector3& _v);
	static Matrix StaticRotation(float x, float y, float z);
	static Matrix StaticRotationX(float x);
	static Matrix StaticRotationY(float y);
	static Matrix StaticRotationZ(float z);
	static Matrix StaticRotationQuaternion(const Vector4& qut);
	static Matrix StaticRotationAxis(const struct Vector3& Axis, float Angle);
	static Matrix StaticTranslation(const struct Vector3& _v);
	static Matrix StaticTranslation(float x, float y, float z);
	static Matrix StaticTranslation(const struct Vector2& _v);
	static Matrix StaticTranslation(float x, float y);
};



