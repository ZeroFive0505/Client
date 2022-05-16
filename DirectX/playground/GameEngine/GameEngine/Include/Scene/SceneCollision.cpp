#include "SceneCollision.h"
#include "../Engine.h"
#include "CollisionSection.h"
#include "../Component/ColliderComponent.h"
#include "../Input.h"
#include "../Scene/Scene.h"

CSceneCollision::CSceneCollision() :
	m_Section(nullptr),
	m_MouseCollision(nullptr)
{

}

CSceneCollision::~CSceneCollision()
{
	size_t size = m_Section->vecSection.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_Section->vecSection[i]);
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->ClearCallback();
	}

	SAFE_DELETE(m_Section);
}

void CSceneCollision::Start()
{
}

bool CSceneCollision::Init()
{
	SetSectionSize(500.0f, 500.0f, 500.0f);
	SetSectionCenter(0.0f, 0.0f, 0.0f);
	SetSectionCount(15, 15, 15);
	CreateSection();

	return true;
}

bool CSceneCollision::CollisionWidget(float deltaTime)
{
	return m_WidgetClick = m_Scene->GetViewport()->CollisionMouse();
}

void CSceneCollision::Collision(float deltaTime)
{
	// ���콺�� �浹�� �ϰ� �ִ� ��ü�� ���Ű� �ȴٸ� ���콺�� �浹�� �浹ü�� ������ nullptr�� �ٲ��ش�.
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			if (*iter == m_MouseCollision)
				m_MouseCollision = nullptr;

			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		iter++;
	}

	// �����ִ� ��� �浹ü���� ���� �������� �־��ش�.
	CheckColliderSection();


	// ���� �浹 ������ ��ġ���� �Ǵ��Ѵ�.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		// ���� �̹� �˻��ߴٸ� �ǳʶڴ�.
		if ((*iter)->GetCurrentSectionCheck())
			continue;

		// ���� �� �浹ü�� �˻縦 �Ϸ��ߴٰ� üũ
		(*iter)->CurrentSectionCheck();

		// ���� �����ӿ� �浹�� �Ǿ��� �浹ü���� �̹� �����ӿ��� ���� ������ �ִ����� �Ǵ��Ѵ�.
		(*iter)->CheckPrevColliderSection();
	}

	// ���� ���콺�� �浹ü�� üũ�Ѵ�.
	CollisionMouse(deltaTime);

	// �浹ü������ �浹�� üüũ�Ѵ�.
	// ��ü Section�� �ݺ��ϸ� �浹�� �����Ѵ�.
	size_t size = m_Section->vecSection.size();

	for (size_t i = 0; i < size; i++)
	{
		// i��° �浹ü �������� �浹 ����
		m_Section->vecSection[i]->Collision(deltaTime);
		// �浹�� �������� ���� �ʱ�ȭ
		m_Section->vecSection[i]->Clear();
	}

	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	// �浹 �׽�Ʈ ������ ��� �浹ü�� ��ȸ�ϸ鼭 ���� �����ӿ��� ������ �浹ü ����Ʈ��
	// ���� ��ȣ ���� �ʱ�ȭ�Ѵ�.
	for (; iter != iterEnd;)
	{
		if ((*iter)->IsActive())
		{
			(*iter)->ClearFrame();
			iter++;
		}
		else
		{
			iter = m_ColliderList.erase(iter);
		}
	}
}

void CSceneCollision::CollisionMouse(float deltaTime)
{
	// UI�� ���콺 �浹�� ó���Ѵ�.
	bool mouseCollision = m_WidgetClick;

	// UI�� ���콺�� �浹���� �ʾҴٸ� ��������� ��ü�� �浹�� �����Ѵ�.
	if (!mouseCollision)
	{
		// ���콺�� �浹������ ��� �浹 ������ �����ϴ����� �Ǵ��Ѵ�.
		// ��, 2D�� 3D�� ���콺 �浹 ����� �ٸ��Ƿ� 2D, 3D�� �����Ͽ� �Ǵ��ؾ� �Ѵ�.
		if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
		{
			Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

			mousePos.x -= m_Section->minSectionCoord.x;
			mousePos.y -= m_Section->minSectionCoord.y;

			int indexX, indexY = 0;

			indexX = (int)(mousePos.x / m_Section->sectionSize.x);
			indexY = (int)(mousePos.y / m_Section->sectionSize.y);

			indexX = indexX < 0 ? -1 : indexX;
			indexY = indexY < 0 ? -1 : indexY;

			indexX = indexX >= m_Section->countX ? -1 : indexX;
			indexY = indexY >= m_Section->countY ? -1 : indexY;

			if (indexX != -1 && indexY != -1)
			{
				CColliderComponent* colliderMouse = m_Section->vecSection[indexY * m_Section->countX + indexX]->CollisionMouse(true, deltaTime);

				if (colliderMouse)
				{
					mouseCollision = true;

					// ���� ���� ����̵Ǿ��� �浹ü�� �ٸ��ٸ�
					// ���Ӱ� �浹�� ��ü ���� �浹 ���� �ݹ��Լ� ȣ��
					if (colliderMouse != m_MouseCollision)
						colliderMouse->CallCollisionMouseCallback(Collision_State::Begin);

					// ���� ���� �浹�Ǿ��� �浹ü�� ���� �ٸ� �浹ü���
					// ���� �浹ü�� ��� �浹 ���� �ݹ� �Լ��� ȣ���Ѵ�.
					if (m_MouseCollision && colliderMouse != m_MouseCollision)
						m_MouseCollision->CallCollisionMouseCallback(Collision_State::End);

					m_MouseCollision = colliderMouse;
				}
			}
		}
		else
		{

		}
	}

	// ���� �浹�� �����ٸ�
	if (!mouseCollision)
	{
		if (m_MouseCollision)
		{
			// ���������� �浹�� �Ǿ��� �浹ü�� �ݹ��Լ� ȣ��
			m_MouseCollision->CallCollisionMouseCallback(Collision_State::End);
			m_MouseCollision = nullptr;
		}
	}
}

void CSceneCollision::SetSectionSize(const Vector3& size)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// ������ ũ�⸦ �����ش�.
	m_Section->sectionSize = size;
	// ��üũ��� ī��Ʈ��ŭ ���ϸ� �ȴ�.
	m_Section->sectionTotalSize = m_Section->sectionSize *
		Vector3((float)m_Section->countX, (float)m_Section->countY, (float)m_Section->countZ);
}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// ���͸�������
	m_Section->sectionSize = Vector3(x, y, z);
	m_Section->sectionTotalSize = m_Section->sectionSize *
		Vector3((float)m_Section->countX, (float)m_Section->countY, (float)m_Section->countZ);
}

void CSceneCollision::SetSectionCenter(const Vector3& center)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// ���͸� �����Ѵ�.
	m_Section->center = center;
	// ���͸� ���������� �ּ�, �ִ� ��ǥ���� ���Ӱ� �����Ѵ�.
	// �ּҴ� �߰����� ��ü ũ�� ������ 2��ŭ �� ��
	m_Section->minSectionCoord = m_Section->center - m_Section->sectionTotalSize / 2.0f;
	// �ִ�� �ּ� ��ǥ���� ��ü ũ�⸸ŭ �����ش�.
	m_Section->maxSectionCoord = m_Section->minSectionCoord + m_Section->sectionTotalSize;
}

void CSceneCollision::SetSectionCenter(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// ���� ��������.
	m_Section->center = Vector3(x, y, z);
	m_Section->minSectionCoord = m_Section->center - m_Section->sectionTotalSize / 2.0f;
	m_Section->maxSectionCoord = m_Section->minSectionCoord + m_Section->sectionTotalSize;
}

void CSceneCollision::SetSectionMinCoord(const Vector3& minCoord)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// �ּ� ��ǥ�� �����Ѵ�
	m_Section->minSectionCoord = minCoord;
	// ��ü ũ����� �����Ѵ�.
	m_Section->sectionTotalSize = m_Section->maxSectionCoord - m_Section->minSectionCoord;
	// ���� �ϳ��� ũ�⸦ ���Ӱ� ���ϰ�
	m_Section->sectionSize = m_Section->sectionTotalSize / Vector3((float)m_Section->countX, (float)m_Section->countY,
		(float)m_Section->countZ);
	// �߽������� ���Ӱ� ���Ѵ�.
	m_Section->center = (m_Section->minSectionCoord + m_Section->maxSectionCoord) / 2.0f;
}

void CSceneCollision::SetSectionMinCoord(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// ���͸�������.
	m_Section->minSectionCoord = Vector3(x, y, z);
	m_Section->sectionTotalSize = m_Section->maxSectionCoord - m_Section->minSectionCoord;
	m_Section->sectionSize = m_Section->sectionTotalSize / Vector3((float)m_Section->countX, (float)m_Section->countY,
		(float)m_Section->countZ);
	m_Section->center = (m_Section->minSectionCoord + m_Section->maxSectionCoord) / 2.0f;
}

void CSceneCollision::SetSectionMaxCoord(const Vector3& maxCoord)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	m_Section->maxSectionCoord = maxCoord;
	m_Section->sectionTotalSize = m_Section->maxSectionCoord - m_Section->minSectionCoord;
	m_Section->sectionSize = m_Section->sectionTotalSize / Vector3((float)m_Section->countX, (float)m_Section->countY,
		(float)m_Section->countZ);
	m_Section->center = (m_Section->minSectionCoord + m_Section->maxSectionCoord) / 2.0f;
}

void CSceneCollision::SetSectionMaxCoord(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	m_Section->maxSectionCoord = Vector3(x, y, z);
	m_Section->sectionTotalSize = m_Section->maxSectionCoord - m_Section->minSectionCoord;
	m_Section->sectionSize = m_Section->sectionTotalSize / Vector3((float)m_Section->countX, (float)m_Section->countY,
		(float)m_Section->countZ);
	m_Section->center = (m_Section->minSectionCoord + m_Section->maxSectionCoord) / 2.0f;
}

void CSceneCollision::SetSectionCount(int countX, int countY, int countZ)
{
	m_Section->countX = countX;
	m_Section->countY = countY;
	m_Section->countZ = countZ;

	m_Section->sectionTotalSize = m_Section->sectionSize * Vector3((float)countX, (float)countY, (float)countZ);
	m_Section->minSectionCoord = m_Section->center - m_Section->sectionTotalSize / 2.0f;
	m_Section->maxSectionCoord = m_Section->minSectionCoord + m_Section->sectionTotalSize;
}

void CSceneCollision::CreateSection()
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 3�� �ݺ������� x, y, z��� ���� �����Ѵ�.
	for (int z = 0; z < m_Section->countZ; z++)
	{
		for (int y = 0; y < m_Section->countY; y++)
		{
			for (int x = 0; x < m_Section->countX; x++)
			{
				CCollisionSection* section = new CCollisionSection;

				section->Init(x, y, z, z * (m_Section->countX * m_Section->countY) + y * m_Section->countX + x,
					m_Section->minSectionCoord, m_Section->maxSectionCoord, m_Section->sectionSize, m_Section->sectionTotalSize);

				m_Section->vecSection.push_back(section);
			}
		}
	}
}

void CSceneCollision::Clear()
{
	size_t size = m_Section->vecSection.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_Section->vecSection[i]);
	}

	SAFE_DELETE(m_Section);
}

void CSceneCollision::AddCollider(CColliderComponent* collider)
{
	// ���� ���� �浹ü ��Ͽ� �߰�
	m_ColliderList.push_back(collider);
}

void CSceneCollision::CheckColliderSection()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		CColliderComponent* collider = *iter;

		if (!collider->IsEnable())
			continue;

		// Collider���� PostUpdate���� Min, Max�� ����� �̹� ������.
		Vector3 Min = collider->GetMin();
		Vector3 Max = collider->GetMax();

		// 0, 0���� ��ǥ�� �����Ѵ�.
		Min -= m_Section->minSectionCoord;
		Max -= m_Section->minSectionCoord;

		int indexMinX, indexMinY, indexMinZ;
		int indexMaxX, indexMaxY, indexMaxZ;

		indexMinX = (int)(Min.x / m_Section->sectionSize.x);
		indexMinY = (int)(Min.y / m_Section->sectionSize.y);
		indexMinZ = (int)(Min.z / m_Section->sectionSize.z);

		indexMaxX = (int)(Max.x / m_Section->sectionSize.x);
		indexMaxY = (int)(Max.y / m_Section->sectionSize.y);
		indexMaxZ = (int)(Max.z / m_Section->sectionSize.z);

		indexMinX = indexMinX < 0 ? 0 : indexMinX;
		indexMinY = indexMinY < 0 ? 0 : indexMinY;
		indexMinZ = indexMinZ < 0 ? 0 : indexMinZ;

		indexMaxX = indexMaxX >= m_Section->countX ? m_Section->countX - 1 : indexMaxX;
		indexMaxY = indexMaxY >= m_Section->countY ? m_Section->countY - 1 : indexMaxY;
		indexMaxZ = indexMaxZ >= m_Section->countZ ? m_Section->countZ - 1 : indexMaxZ;

		// ������ ���� �ε����� �浹ü�� ������ ��Ͻ�Ų��.
		for (int z = indexMinZ; z <= indexMaxZ; z++)
		{
			for (int y = indexMinY; y <= indexMaxY; y++)
			{
				for (int x = indexMinX; x <= indexMaxX; x++)
				{
					int index = z * (m_Section->countX * m_Section->countY) + y * m_Section->countX + x;

					m_Section->vecSection[index]->AddCollider(collider);
				}
			}
		}
	}
}