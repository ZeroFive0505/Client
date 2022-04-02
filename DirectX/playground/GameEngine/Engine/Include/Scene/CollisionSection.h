#pragma once

#include "../GameInfo.h"

/// <summary>
/// 현재 충돌 영역에 대한 정보를 저장하는 클래스
/// </summary>
class CCollisionSection
{
	friend class CSceneCollision;

private:
	CCollisionSection();
	~CCollisionSection();

private:
	std::vector<class CColliderComponent*> m_vecCollider;
	Vector3 m_SectionSize;
	Vector3 m_SectionTotalSize;
	Vector3 m_Min;
	Vector3 m_Max;
	int m_IndexX;
	int m_IndexY;
	int m_IndexZ;
	int m_Index;

public:
	void Init(int indexX, int indexY, int indexZ, int index,
		const Vector3& Min, const Vector3& Max, 
		const Vector3& sectionSize, const Vector3& sectionTotalSize);
	void Clear();
	void AddCollider(class CColliderComponent* collider);
	void Collision(float deltaTime);
	class CColliderComponent* CollisionMouse(bool Is2D, float deltaTime);

private:
	static int SortY(const void* src, const void* dest);
};

