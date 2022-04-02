#pragma once

#include "../GameInfo.h"
/// <summary>
/// �� ���ο� ��� �浹ü�� �̰����� ��� �浹 �׽�Ʈ�� �����Ѵ�.
/// </summary>
class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	// ��� ���� �����ִ���
	class CScene* m_Scene;
	// �Ϲ� �浹ü�� ��Ƴ��� ����
	std::vector<class CCollider*> m_vecCollider;
	
	// UI �浹ü�� ��Ƴ��� ����
	std::vector<class CUIWindow*> m_vecUIWindow;

	// ���콺�� ȣ�������� ����
	class CUIWidget* m_MouseHoverdWidget;
	// ���콺�� ������ ����
	class CUIWidget* m_SelectWidget;
	// ���� ���콺�� �浹�� ������Ʈ
	class CCollider* m_MouseCollision;

public:
	void SetSelectWidget(class CUIWidget* Widget)
	{
		m_SelectWidget = Widget;
	}

	void ClearMouseCollision(class CCollider* Collider)
	{
		if (m_MouseCollision == Collider)
			m_MouseCollision = nullptr;
	}

public:
	// �Ϲ� �浹ü�� �߰��ϴ� �Լ�
	void AddCollider(class CCollider* Collider);
	// UI �浹ü�� �߰��ϴ� �Լ�.
	void AddUIWindow(class CUIWindow* Window);
	// �Ϲ� �浹ü�鰣�� �浹üũ�� �����ϴ� �Լ�
	void Collision(float DeltaTime);
	// UI�� ���콺 �浹�� �˻��ϴ� �Լ�.
	void CollisionMouse(float DeltaTime);

private:
	static int SortY(const void* Src, const void* Dest);
};

