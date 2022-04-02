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

	// �� �浹������ ������ ũ�⸦ 100���� ��´�.
	m_vecCollider.reserve(100);
}

void CCollisionSection::Clear()
{
	m_vecCollider.clear();
}

void CCollisionSection::AddCollider(CColliderComponent* collider)
{
	m_vecCollider.push_back(collider);

	// �� ���ǿ� ���Ͽ� �߰��Ǹ鼭 �浹ü�� �� ���� ��ȣ�� �־��ش�.
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

			// ���� �����ӿ��� �浹�� üũ������ �ִٸ� �ǳʶڴ�.
			if (src->CheckCurrentFrameCollision(dest))
				continue;

			sCollisionProfile* srcProfile = src->GetCollisionProfile();
			sCollisionProfile* destProfile = dest->GetCollisionProfile();

			// ���� ���� ä�ΰ��� �浹 ���¸� ���Ѵ�.

			// ���� ���߿� �ϳ��� ���ö��
			if (srcProfile->vecInteraction[(int)destProfile->channel] == Collision_Interaction::Ignore ||
				destProfile->vecInteraction[(int)srcProfile->channel] == Collision_Interaction::Ignore)
				continue;

			if (src->Collision(dest))
			{
				// ����, �� �浹�� ������� �Ǵ��Ѵ�.
				// ��, ���� �����ӿ� �浹�� ��Ͽ� ���ٸ� ���� �� �浹�� ���۵� ��
				if (!src->CheckPrevCollision(dest))
				{
					// �÷� �߰��ϰ� �ݹ��Լ� ȣ��
					src->AddPrevCollision(dest);
					dest->AddPrevCollision(src);

					src->CallCollisionCallback(Collision_State::Begin);
					dest->CallCollisionCallback(Collision_State::Begin);
				}

				// ���� ������ �浹 ����Ʈ�� �߰��Ѵ�.
				src->AddCurrentFrameCollision(dest);
				dest->AddCurrentFrameCollision(src);
			}
			// ���� ���� �����ӿ��� �浹�� ���� �ʾ����� �ٷ� ���� �����ӿ��� ���� �浹�� �־���
			// ���� �浹�� �Ͼ�� �ʾҴٸ� �� �浹�� ���� �����̴�.
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

		// ��� �浹ü���� y�� �����Ѵ�.
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
