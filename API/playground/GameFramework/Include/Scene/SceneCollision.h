#pragma once

#include "../GameInfo.h"
/// <summary>
/// 씬 내부에 모든 충돌체를 이곳으로 모아 충돌 테스트를 진행한다.
/// </summary>
class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	// 어느 씬에 속해있는지
	class CScene* m_Scene;
	// 일반 충돌체를 모아놓는 벡터
	std::vector<class CCollider*> m_vecCollider;
	
	// UI 충돌체를 모아놓는 벡터
	std::vector<class CUIWindow*> m_vecUIWindow;

	// 마우스가 호버링중인 위젯
	class CUIWidget* m_MouseHoverdWidget;
	// 마우스가 선택한 위젯
	class CUIWidget* m_SelectWidget;
	// 현재 마우스와 충돌한 오브젝트
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
	// 일반 충돌체를 추가하는 함수
	void AddCollider(class CCollider* Collider);
	// UI 충돌체를 추가하는 함수.
	void AddUIWindow(class CUIWindow* Window);
	// 일반 충돌체들간의 충돌체크를 시작하는 함수
	void Collision(float DeltaTime);
	// UI와 마우스 충돌을 검사하는 함수.
	void CollisionMouse(float DeltaTime);

private:
	static int SortY(const void* Src, const void* Dest);
};

