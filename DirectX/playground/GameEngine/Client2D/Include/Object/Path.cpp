#include "Path.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Line.h"
#include "Scene/SceneManager.h"

CPath::CPath()
{
}

CPath::CPath(const CPath* obj)
{
}

CPath::~CPath()
{
}

bool CPath::Init()
{
	m_Radius = 10.0f;

	sResolution rs = CEngine::GetInst()->GetResolution();

	Vector2 point;

	point = Vector2(0.0f, (float)rs.height / 2.0f);

	m_vecPoints.push_back(point);

	point = Vector2((float)rs.width * 0.25f, (float)rs.height / 4.0f);

	m_vecPoints.push_back(point);

	point = Vector2((float)rs.width * 0.5f, (float)rs.height / 1.5f);

	m_vecPoints.push_back(point);

	point = Vector2((float)rs.width, (float)rs.height / 2.0f);

	return true;
}
