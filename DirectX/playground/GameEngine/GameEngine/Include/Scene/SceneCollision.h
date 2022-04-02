#pragma once

#include "../GameInfo.h"

#include "../GameInfo.h"

// ������ �����ؼ� �浹ü�� �Ҵ��Ҷ� ���Ǵ� ����ü
struct sCollisionSectionInfo
{
	std::vector<class CCollisionSection*> vecSection;
	Vector3 sectionSize; // ���� 1ĭ�� ũ��
	Vector3 center;	// ��ü ���������� �߽�
	Vector3 sectionTotalSize; // ��ü ���� ũ��
	Vector3 minSectionCoord; // ��ü ���������� �ּҰ�
	Vector3 maxSectionCoord; // ��ü ���������� �ִ밪
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

// ���� ����ũ��� ���� �浹ü�� ��Ͻ�Ű�� Ŭ����
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