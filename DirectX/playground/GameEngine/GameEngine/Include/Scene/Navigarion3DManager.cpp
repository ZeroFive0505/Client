#include "Navigarion3DManager.h"

CNavigarion3DManager::CNavigarion3DManager()
{
}

CNavigarion3DManager::~CNavigarion3DManager()
{
}

void CNavigarion3DManager::SetNavData(CLandScape* navData)
{
	m_NavData = navData;
}

void CNavigarion3DManager::AddNavResult(const sNavResultData& navData)
{
}

float CNavigarion3DManager::GetLandScapeHeight(const Vector3& pos)
{
	if (!m_NavData)
		return pos.y;

	return m_NavData->GetHeight(pos);
}

void CNavigarion3DManager::Start()
{
}

bool CNavigarion3DManager::Init()
{
	return true;
}

void CNavigarion3DManager::Update(float deltaTime)
{
}
