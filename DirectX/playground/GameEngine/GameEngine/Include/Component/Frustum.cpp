#include "Frustum.h"

CFrustum::CFrustum()
{
	m_Pos[0] = Vector3(-1.0f, 1.0f, 0.0f);
	m_Pos[1] = Vector3(1.0f, 1.0f, 0.0f);
	m_Pos[2] = Vector3(-1.0f, -1.0f, 0.0f);
	m_Pos[3] = Vector3(1.0f, -1.0f, 0.0f);

	m_Pos[4] = Vector3(-1.0f, 1.0f, 1.0f);
	m_Pos[5] = Vector3(1.0f, 1.0f, 1.0f);
	m_Pos[6] = Vector3(-1.0f, -1.0f, 1.0f);
	m_Pos[7] = Vector3(1.0f, -1.0f, 1.0f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::Update(Matrix matVP)
{
	// ������������� 8�� ������ ��ġ�� ���Ѵ�.
	Vector3 pos[8];

	// ������� ���Ѵ�.
	matVP.Inverse();

	// ���忡���� ��ġ
	for (int i = 0; i < 8; i++)
	{
		pos[i] = m_Pos[i].TransformCoord(matVP);
	}


	// LEFT
	m_Plane[(int)Frustum_Plane_Direction::LEFTPLANE] = XMPlaneFromPoints(pos[4].Convert(), pos[0].Convert(),
		pos[2].Convert());

	// RIGHT
	m_Plane[(int)Frustum_Plane_Direction::RIGHTPLANE] = XMPlaneFromPoints(pos[1].Convert(), pos[5].Convert(),
		pos[7].Convert());

	// TOP
	m_Plane[(int)Frustum_Plane_Direction::TOPPLANE] = XMPlaneFromPoints(pos[4].Convert(), pos[5].Convert(),
		pos[1].Convert());

	// BOTTOM
	m_Plane[(int)Frustum_Plane_Direction::BOTTOMPLANE] = XMPlaneFromPoints(pos[2].Convert(), pos[3].Convert(),
		pos[7].Convert());

	// NEAR
	m_Plane[(int)Frustum_Plane_Direction::NEARPLANE] = XMPlaneFromPoints(pos[0].Convert(), pos[1].Convert(),
		pos[3].Convert());

	// FAR
	m_Plane[(int)Frustum_Plane_Direction::FARPLANE] = XMPlaneFromPoints(pos[5].Convert(), pos[4].Convert(),
		pos[6].Convert());
}

bool CFrustum::FrustumInPoint(const Vector3& point)
{
	// ���� ��� ����� ���� ���ʹ� �ܺη� ���ϰ� �ִ�.
	// ���� ��� ��鿡�� ������ ���� ������ ���;��Ѵ�.
	for (int i = 0; i < (int)Frustum_Plane_Direction::MAX; i++)
	{
		float dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), point.Convert()));

		if (dot > 0.0f)
			return true;
	}

	return false;
}

bool CFrustum::FrustumInSphere(const sSphereInfo& sphere)
{
	// ���� ��� ����� ���� ���ʹ� �ܺη� ���ϰ� �ִ�.
	// ���� ��� ��鿡�� ������ ���� ������ ���;��Ѵ�.
	for (int i = 0; i < (int)Frustum_Plane_Direction::MAX; i++)
	{
		float dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), sphere.center.Convert()));

		// ���� ��� ������ ��ŭ �İ����� ��츦 �����Ѵ�. ���� ���������� ũ��
		// ������ �������� �������� ����ٴ� ��, ��鿡 �� ������������ 0�̴�.
		if (dot > sphere.radius)
			return true;
	}

	return false;
}
