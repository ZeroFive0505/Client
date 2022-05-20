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
	// 월드공간에서의 8개 정점의 위치를 구한다.
	Vector3 pos[8];

	// 역행렬을 구한다.
	matVP.Inverse();

	// 월드에서의 위치
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
	// 현재 모든 평번의 법선 벡터는 외부로 향하고 있다.
	// 따라서 모든 평면에서 내적의 값이 음수가 나와야한다.
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
	// 현재 모든 평번의 법선 벡터는 외부로 향하고 있다.
	// 따라서 모든 평면에서 내적의 값이 음수가 나와야한다.
	for (int i = 0; i < (int)Frustum_Plane_Direction::MAX; i++)
	{
		float dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), sphere.center.Convert()));

		// 구의 경우 반지름 만큼 파고들었을 경우를 가정한다. 따라서 반지름보다 크면
		// 완전히 프러스텀 영역에서 벗어났다는 뜻, 평면에 딱 걸쳐있을경우는 0이다.
		if (dot > sphere.radius)
			return true;
	}

	return false;
}
