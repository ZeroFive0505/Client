#include "Camera.h"

CCamera::CCamera() :
	m_Resolution(512.0f, 448.0f),
	m_WordResolution(1024.0f, 1024.0f),
	m_InfiniteScroll(false)
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init()
{
	return true;
}

void CCamera::Update(float deltaTime)
{
	if (m_Target)
	{
		if (!m_Target->IsActive())
			m_Target = nullptr;
		else
		{
			m_Pos = m_Target->GetPos() - m_TargetPivot * m_Resolution + m_TargetOffset;
		}
	}


	if (!m_InfiniteScroll)
	{
		if (m_Pos.x < 0.0f)
			m_Pos.x = 0.0f;
		else if (m_Pos.x + m_Resolution.x > m_WordResolution.x)
			m_Pos.x = m_WordResolution.x - m_Resolution.x;

		if (m_Pos.y < 0.0f)
			m_Pos.y = 0.0f;
		else if (m_Pos.y + m_Resolution.y > m_WordResolution.y)
			m_Pos.y = m_WordResolution.y - m_Resolution.y;
	}
	else
	{
		m_Pos.x = fmodf(m_Pos.x, m_WordResolution.x);
		m_Pos.y = fmodf(m_Pos.y, m_WordResolution.y);
	}
}
