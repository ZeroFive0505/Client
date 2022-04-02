#include "FloorObject.h"
#include "../Scene/RaceScene.h"

std::vector<CSharedPtr<CFloorObject>> CFloorObject::vecChangesQueue;

CRaceScene* CFloorObject::m_StaticScene = nullptr;


CFloorObject::CFloorObject(const Vector2& pos, const Vector2& size, EFloorObjectType type, const int mapWidth, const int mapHeight, const EFloorObjectOrientation orientation, class CRaceScene* scene) :
	m_TopLeftPixel(pos), m_Size(size), m_Type(type)
{
	float scaleX = 1.0f / mapWidth;
	float scaleY = 1.0f / mapHeight;

	m_Hitbox.leftTop = Vector2(pos.x * scaleX, pos.y * scaleY);
	m_Hitbox.rightBottom = m_Hitbox.leftTop + Vector2(size.x * scaleX, size.y * scaleY);

	m_Orientation = orientation;
}

CFloorObject::~CFloorObject()
{
	m_StaticScene = nullptr;

	vecChangesQueue.clear();
}

// 현재 상태를 반환한다.
EFloorObjectState CFloorObject::GetState() const
{
	return m_State;
}

// 상태가 변했을때의 상태를 설정한다.
void CFloorObject::SetState(EFloorObjectState state)
{
	// 만약 초기 상태라면
	if (state == EFloorObjectState::INITIAL)
	{
		// 초기 상태의 값을 가져온다.
		// 각각 바닥 오브젝트마다 다른 초기상태를 가질 수도 있음.
		m_State = GetInitState();
	}
	else
	{
		// 상태를 등록한다.
		m_State = state;
	}

	// 변화 큐에 등록한다.
	CFloorObject::vecChangesQueue.push_back(this);
}


void CFloorObject::Update(float deltaTime)
{
}

// 변화 적용
void CFloorObject::DefaultApplyChanges(const CFloorObject* that)
{
	m_StaticScene->SetFloorObjectUpdated(true);
}

// 사각형 vs 점 충돌
bool CFloorObject::CollidesWith(const CSharedPtr<CDriver>& driver) const
{
	if (m_Hitbox.leftTop.x <= driver->GetPos().x && driver->GetPos().x <= m_Hitbox.rightBottom.x &&
		m_Hitbox.leftTop.y <= driver->GetPos().y && driver->GetPos().y <= m_Hitbox.rightBottom.y)
		return true;
	else
		return false;
}


// 큐를 순회하면서 모든 변화를 적용 시킨다.
bool CFloorObject::ApplyAllChanges()
{
	if (vecChangesQueue.empty())
		return false;
	else
	{
		for (const auto& obj : vecChangesQueue)
		{
			obj->ApplyChanges();
		}

		vecChangesQueue.clear();

		return true;
	}
}

void CFloorObject::ResetChanges()
{
	vecChangesQueue.clear();
}
