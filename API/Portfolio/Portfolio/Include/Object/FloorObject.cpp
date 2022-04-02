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

// ���� ���¸� ��ȯ�Ѵ�.
EFloorObjectState CFloorObject::GetState() const
{
	return m_State;
}

// ���°� ���������� ���¸� �����Ѵ�.
void CFloorObject::SetState(EFloorObjectState state)
{
	// ���� �ʱ� ���¶��
	if (state == EFloorObjectState::INITIAL)
	{
		// �ʱ� ������ ���� �����´�.
		// ���� �ٴ� ������Ʈ���� �ٸ� �ʱ���¸� ���� ���� ����.
		m_State = GetInitState();
	}
	else
	{
		// ���¸� ����Ѵ�.
		m_State = state;
	}

	// ��ȭ ť�� ����Ѵ�.
	CFloorObject::vecChangesQueue.push_back(this);
}


void CFloorObject::Update(float deltaTime)
{
}

// ��ȭ ����
void CFloorObject::DefaultApplyChanges(const CFloorObject* that)
{
	m_StaticScene->SetFloorObjectUpdated(true);
}

// �簢�� vs �� �浹
bool CFloorObject::CollidesWith(const CSharedPtr<CDriver>& driver) const
{
	if (m_Hitbox.leftTop.x <= driver->GetPos().x && driver->GetPos().x <= m_Hitbox.rightBottom.x &&
		m_Hitbox.leftTop.y <= driver->GetPos().y && driver->GetPos().y <= m_Hitbox.rightBottom.y)
		return true;
	else
		return false;
}


// ť�� ��ȸ�ϸ鼭 ��� ��ȭ�� ���� ��Ų��.
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
