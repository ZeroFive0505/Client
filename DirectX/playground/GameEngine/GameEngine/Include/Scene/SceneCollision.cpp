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
	// 마우스와 충돌을 하고 있던 물체가 제거가 된다면 마우스와 충돌한 충돌체의 정보를 nullptr로 바꿔준다.
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

	// 씬에있는 모든 충돌체들을 각자 영역으로 넣어준다.
	CheckColliderSection();


	// 현재 충돌 영역이 겹치는지 판단한다.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		// 만약 이미 검사했다면 건너뛴다.
		if ((*iter)->GetCurrentSectionCheck())
			continue;

		// 현재 이 충돌체는 검사를 완료했다고 체크
		(*iter)->CurrentSectionCheck();

		// 이전 프레임에 충돌이 되었던 충돌체들이 이번 프레임에도 같은 영역에 있는지를 판단한다.
		(*iter)->CheckPrevColliderSection();
	}

	// 먼저 마우스와 충돌체를 체크한다.
	CollisionMouse(deltaTime);

	// 충돌체끼리의 충돌을 체체크한다.
	// 전체 Section을 반복하며 충돌을 진행한다.
	size_t size = m_Section->vecSection.size();

	for (size_t i = 0; i < size; i++)
	{
		// i번째 충돌체 영역에서 충돌 진행
		m_Section->vecSection[i]->Collision(deltaTime);
		// 충돌을 끝냈으니 버켓 초기화
		m_Section->vecSection[i]->Clear();
	}

	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	// 충돌 테스트 종료후 모든 충돌체를 순회하면서 현재 프레임에서 생성된 충돌체 리스트와
	// 섹션 번호 등을 초기화한다.
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
	// UI와 마우스 충돌을 처리한다.
	bool mouseCollision = m_WidgetClick;

	// UI와 마우스와 충돌되지 않았다면 월드공간의 물체와 충돌을 진행한다.
	if (!mouseCollision)
	{
		// 마우스가 충돌영역중 어느 충돌 영역에 존재하는지를 판단한다.
		// 단, 2D와 3D는 마우스 충돌 방식이 다르므로 2D, 3D를 구분하여 판단해야 한다.
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

					// 만약 전에 등록이되었던 충돌체와 다르다면
					// 새롭게 충돌된 물체 따라서 충돌 시작 콜백함수 호출
					if (colliderMouse != m_MouseCollision)
						colliderMouse->CallCollisionMouseCallback(Collision_State::Begin);

					// 만약 전에 충돌되었던 충돌체와 전혀 다른 충돌체라면
					// 현재 충돌체의 경우 충돌 종료 콜백 함수를 호출한다.
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

	// 만약 충돌이 없었다면
	if (!mouseCollision)
	{
		if (m_MouseCollision)
		{
			// 마지막으로 충돌이 되었던 충돌체의 콜백함수 호출
			m_MouseCollision->CallCollisionMouseCallback(Collision_State::End);
			m_MouseCollision = nullptr;
		}
	}
}

void CSceneCollision::SetSectionSize(const Vector3& size)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 섹션의 크기를 정해준다.
	m_Section->sectionSize = size;
	// 전체크기는 카운트만큼 곱하면 된다.
	m_Section->sectionTotalSize = m_Section->sectionSize *
		Vector3((float)m_Section->countX, (float)m_Section->countY, (float)m_Section->countZ);
}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 위와마찬가지
	m_Section->sectionSize = Vector3(x, y, z);
	m_Section->sectionTotalSize = m_Section->sectionSize *
		Vector3((float)m_Section->countX, (float)m_Section->countY, (float)m_Section->countZ);
}

void CSceneCollision::SetSectionCenter(const Vector3& center)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 센터를 수정한다.
	m_Section->center = center;
	// 센터를 수정했으니 최소, 최대 좌표또한 새롭게 갱신한다.
	// 최소는 중간에서 전체 크기 나누기 2만큼 뺀 곳
	m_Section->minSectionCoord = m_Section->center - m_Section->sectionTotalSize / 2.0f;
	// 최대는 최소 좌표에서 전체 크기만큼 더해준다.
	m_Section->maxSectionCoord = m_Section->minSectionCoord + m_Section->sectionTotalSize;
}

void CSceneCollision::SetSectionCenter(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 위외 마찬가지.
	m_Section->center = Vector3(x, y, z);
	m_Section->minSectionCoord = m_Section->center - m_Section->sectionTotalSize / 2.0f;
	m_Section->maxSectionCoord = m_Section->minSectionCoord + m_Section->sectionTotalSize;
}

void CSceneCollision::SetSectionMinCoord(const Vector3& minCoord)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 최소 좌표를 수정한다
	m_Section->minSectionCoord = minCoord;
	// 전체 크기또한 수정한다.
	m_Section->sectionTotalSize = m_Section->maxSectionCoord - m_Section->minSectionCoord;
	// 섹션 하나의 크기를 새롭게 구하고
	m_Section->sectionSize = m_Section->sectionTotalSize / Vector3((float)m_Section->countX, (float)m_Section->countY,
		(float)m_Section->countZ);
	// 중심점또한 새롭게 구한다.
	m_Section->center = (m_Section->minSectionCoord + m_Section->maxSectionCoord) / 2.0f;
}

void CSceneCollision::SetSectionMinCoord(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new sCollisionSectionInfo;

	// 위와마찬가지.
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

	// 3중 반복문으로 x, y, z모든 곳을 분할한다.
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
	// 현재 씬의 충돌체 목록에 추가
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

		// Collider들은 PostUpdate에서 Min, Max가 계산이 이미 끝났다.
		Vector3 Min = collider->GetMin();
		Vector3 Max = collider->GetMax();

		// 0, 0으로 좌표를 수정한다.
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

		// 위에서 구한 인덱스로 충돌체의 영역에 등록시킨다.
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