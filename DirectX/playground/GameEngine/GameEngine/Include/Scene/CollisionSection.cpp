#include "CollisionSection.h"
#include "../Input.h"
#include "../Component/ColliderComponent.h"

CCollisionSection::CCollisionSection()
{
}

CCollisionSection::~CCollisionSection()
{
}

void CCollisionSection::Init(int indexX, int indexY, int indexZ, int index, 
	const Vector3& Min, const Vector3& Max, const Vector3& sectionSize, const Vector3& sectionTotalSize)
{
	m_IndexX = indexX;
	m_IndexY = indexY;
	m_IndexZ = indexZ;
	m_Index = index;
	m_Min = Min;
	m_Max = Max;
	m_SectionSize = sectionSize;
	m_SectionTotalSize = sectionTotalSize;

	// 이 충돌영역의 버켓의 크기를 100으로 잡는다.
	m_vecCollider.reserve(100);
}

void CCollisionSection::Clear()
{
	m_vecCollider.clear();
}

void CCollisionSection::AddCollider(CColliderComponent* collider)
{
	m_vecCollider.push_back(collider);

	// 이 섹션에 버켓에 추가되면서 충돌체에 이 섹션 번호를 넣어준다.
	collider->AddSectionIndex(m_Index);
}

void CCollisionSection::Collision(float deltaTime)
{
	size_t size = m_vecCollider.size();

	if (size < 2)
		return;

	for (size_t i = 0; i < size - 1; i++)
	{
		CColliderComponent* src = m_vecCollider[i];

		for (size_t j = i + 1; j < size; j++)
		{
			CColliderComponent* dest = m_vecCollider[j];

			// 현재 프레임에서 충돌을 체크한적이 있다면 건너뛴다.
			if (src->CheckCurrentFrameCollision(dest))
				continue;

			sCollisionProfile* srcProfile = src->GetCollisionProfile();
			sCollisionProfile* destProfile = dest->GetCollisionProfile();

			// 서로 상대방 채널과의 충돌 상태를 비교한다.

			// 만약 둘중에 하나라도 무시라면
			if (srcProfile->vecInteraction[(int)destProfile->channel] == Collision_Interaction::Ignore ||
				destProfile->vecInteraction[(int)srcProfile->channel] == Collision_Interaction::Ignore)
				continue;

			if (src->Collision(dest))
			{
				// 지금, 막 충돌이 됬는지를 판단한다.
				// 즉, 이전 프레임에 충돌된 목록에 없다면 지금 막 충돌이 시작된 것
				if (!src->CheckPrevCollision(dest))
				{
					// 시로 추가하고 콜백함수 호출
					src->AddPrevCollision(dest);
					dest->AddPrevCollision(src);

					src->CallCollisionCallback(Collision_State::Begin);
					dest->CallCollisionCallback(Collision_State::Begin);
				}

				// 현재 프레임 충돌 리스트에 추가한다.
				src->AddCurrentFrameCollision(dest);
				dest->AddCurrentFrameCollision(src);
			}
			// 만약 현재 프레임에는 충돌이 되지 않았지만 바로 이전 프레임에서 서로 충돌이 있었고
			// 지금 충돌이 일어나지 않았다면 막 충돌이 끝난 상태이다.
			else if (src->CheckPrevCollision(dest))
			{
				src->DeletePrevCollision(dest);
				dest->DeletePrevCollision(src);

				src->CallCollisionCallback(Collision_State::End);
				dest->CallCollisionCallback(Collision_State::End);
			}
		}
	}
}

CColliderComponent* CCollisionSection::CollisionMouse(bool Is2D, float deltaTime)
{
	if (Is2D)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

		size_t size = m_vecCollider.size();

		// 모든 충돌체들을 y축 정렬한다.
		if (size > 1)
			qsort(&m_vecCollider[0], size, (size_t)sizeof(CColliderComponent*), CCollisionSection::SortY);

		for (size_t i = 0; i < size; i++)
		{
			if (m_vecCollider[i]->CollisionMouse(mousePos))
				return m_vecCollider[i];
		}
	}
	else
	{

	}

	return nullptr;
}

int CCollisionSection::SortY(const void* src, const void* dest)
{
	CColliderComponent* srcCollider = *((CColliderComponent**)src);
	CColliderComponent* destCollider = *((CColliderComponent**)dest);

	if (srcCollider->GetMin().y < destCollider->GetMin().y)
		return -1;
	else if (srcCollider->GetMin().y > destCollider->GetMin().y)
		return 1;
	
	return 0;
}
