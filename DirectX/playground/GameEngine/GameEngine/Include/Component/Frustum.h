#pragma once

#include "../GameInfo.h"

enum class Frustum_Plane_Direction
{
	LEFTPLANE,
	RIGHTPLANE,
	TOPPLANE,
	BOTTOMPLANE,
	NEARPLANE,
	FARPLANE,
	MAX
};

class CFrustum
{
	friend class CCameraComponent;

private:
	CFrustum();
	~CFrustum();

private:
	// 프러스텀을 정의하기 위한 8개의 정점
	Vector3 m_Pos[8];
	// 6개의 평면 방정식
	Vector4 m_Plane[(int)Frustum_Plane_Direction::MAX];

public:
	void Update(Matrix matVP);
	bool FrustumInPoint(const Vector3& point);
	bool FrustumInSphere(const sSphereInfo& sphere);
};

