
#include "Frustum.h"

CFrustum::CFrustum()
{
	m_Pos[0] = Vector3(-1.f, 1.f, 0.f);
	m_Pos[1] = Vector3(1.f, 1.f, 0.f);
	m_Pos[2] = Vector3(-1.f, -1.f, 0.f);
	m_Pos[3] = Vector3(1.f, -1.f, 0.f);

	m_Pos[4] = Vector3(-1.f, 1.f, 1.f);
	m_Pos[5] = Vector3(1.f, 1.f, 1.f);
	m_Pos[6] = Vector3(-1.f, -1.f, 1.f);
	m_Pos[7] = Vector3(1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::Update(Matrix matVP)
{
	// 월드공간에서의 8개 위치를 구한다.
	Vector3	Pos[8];

	// 역행렬을 구한다.
	matVP.Inverse();

	for (int i = 0; i < 8; ++i)
	{
		Pos[i] = m_Pos[i].TransformCoord(matVP);
	}

	// Left
	m_Plane[(int)Frustum_Plane_Dir::Left] = XMPlaneFromPoints(Pos[4].Convert(), Pos[0].Convert(),
		Pos[2].Convert());

	// Right
	m_Plane[(int)Frustum_Plane_Dir::Right] = XMPlaneFromPoints(Pos[1].Convert(), Pos[5].Convert(),
		Pos[7].Convert());

	// Top
	m_Plane[(int)Frustum_Plane_Dir::Top] = XMPlaneFromPoints(Pos[4].Convert(), Pos[5].Convert(),
		Pos[1].Convert());

	// Bottom
	m_Plane[(int)Frustum_Plane_Dir::Bottom] = XMPlaneFromPoints(Pos[2].Convert(), Pos[3].Convert(),
		Pos[7].Convert());

	// Near
	m_Plane[(int)Frustum_Plane_Dir::Near] = XMPlaneFromPoints(Pos[0].Convert(), Pos[1].Convert(),
		Pos[3].Convert());

	// Far
	m_Plane[(int)Frustum_Plane_Dir::Far] = XMPlaneFromPoints(Pos[5].Convert(), Pos[4].Convert(),
		Pos[6].Convert());
}

bool CFrustum::FrustumInPoint(const Vector3& Point)
{
	for (int i = 0; i < (int)Frustum_Plane_Dir::Max; ++i)
	{
		float	Dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), Point.Convert()));

		if (Dot > 0.f)
			return true;
	}

	return false;
}

bool CFrustum::FrustumInSphere(const SphereInfo& Sphere)
{
	for (int i = 0; i < (int)Frustum_Plane_Dir::Max; ++i)
	{
		float	Dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), Sphere.Center.Convert()));

		if (Dot > Sphere.Radius)
			return true;
	}

	return false;
}
