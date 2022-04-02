#pragma once

#include "../GameInfo.h"

#include "../GameInfo.h"

// 공간을 분할해서 충돌체를 할당할때 사용되는 구조체
struct sCollisionSectionInfo
{
	std::vector<class CCollisionSection*> vecSection;
	Vector3 sectionSize; // 영역 1칸의 크기
	Vector3 center;	// 전체 영역에서의 중심
	Vector3 sectionTotalSize; // 전체 영역 크기
	Vector3 minSectionCoord; // 전체 영역에서의 최소값
	Vector3 maxSectionCoord; // 전체 영역에서의 최대값
	int countX;
	int countY;
	int countZ;

	sCollisionSectionInfo() :
		countX(1),
		countY(1),
		countZ(1),
		sectionSize(1000.0f, 1000.0f, 1000.0f),
		sectionTotalSize(1000.0f, 1000.0f, 1.0f),
		minSectionCoord(-500.0f, -500.0f, -0.5f),
		maxSectionCoord(500.0f, 500.0f, 0.5f)
	{}
};

// 씬을 적정크기로 나눠 충돌체를 등록시키는 클래스
class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	class CScene* m_Scene;

private:
	sCollisionSectionInfo* m_Section;
	std::list<CSharedPtr<class CColliderComponent>> m_ColliderList;
	CSharedPtr<class CColliderComponent> m_MouseCollision;
	bool m_WidgetClick;

public:
	void Start();
	bool Init();
	bool CollisionWidget(float deltaTime);
	void Collision(float deltaTime);

protected:
	void CollisionMouse(float deltaTime);

public:
	void SetSectionSize(const Vector3& size);
	void SetSectionSize(float x, float y, float z = 1.0f);
	void SetSectionCenter(const Vector3& center);
	void SetSectionCenter(float x, float y, float z);
	void SetSectionMinCoord(const Vector3& minCoord);
	void SetSectionMinCoord(float x, float y, float z);
	void SetSectionMaxCoord(const Vector3& maxCoord);
	void SetSectionMaxCoord(float x, float y, float z);
	void SetSectionCount(int countX, int countY, int countZ = 1);
	void CreateSection();
	void Clear();
	void AddCollider(class CColliderComponent* collider);

private:
	void CheckColliderSection();
};