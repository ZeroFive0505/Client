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
	// ���������� �����ϱ� ���� 8���� ����
	Vector3 m_Pos[8];
	// 6���� ��� ������
	Vector4 m_Plane[(int)Frustum_Plane_Direction::MAX];

public:
	void Update(Matrix matVP);
	bool FrustumInPoint(const Vector3& point);
	bool FrustumInSphere(const sSphereInfo& sphere);
};

