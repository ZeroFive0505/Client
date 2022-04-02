#pragma once

#include "../Ref.h"
#include "UIWidget.h"

/// <summary>
/// 게임 오브젝트가 가질 수 있는 위젯 컴포넌트
/// 이 위젯 컴포넌트는 스크린좌표가 아닌 월드 좌표를 이용해서 출력된다.
/// 즉 소유자의 상대적인 위치에 따라 위치가 계속 변한다.
/// </summary>
class CWidgetComponent :
	public CRef
{
	friend class CGameObject;

private:
	CWidgetComponent();
	CWidgetComponent(const CWidgetComponent& widget);
	~CWidgetComponent();

private:
	// 이 위젯 컴포너는트를 가지고 있는 소유주
	class CGameObject* m_Owner;
	// 현재 어떤 씬에 소속되있는지
	class CScene* m_Scene;
	// 가지고 있는 위젯
	CSharedPtr<CUIWidget> m_Widget;
	// 위치
	Vector2 m_Pos;
	bool m_Visibility;

public:
	Vector2 GetPos() const
	{
		return m_Pos;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetOwner(class CGameObject* Owner)
	{
		m_Owner = Owner;
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetVisibility(bool Visibilty)
	{
		m_Visibility = Visibilty;
	}

	bool GetVisibility() const
	{
		return m_Visibility;
	}

	void SetWidget(CUIWidget* Widget)
	{
		m_Widget;
	}

	CUIWidget* GetWidget() const
	{
		return m_Widget;
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(HDC hDC);
	CWidgetComponent* Clone();

public:
	/// <summary>
	/// 현재 위젯 컴포넌트에 새로운 위젯을 등록(추가) 시킨다.
	/// </summary>
	/// <typeparam name="T">생성시키고자 하는 위젯의 타입</typeparam>
	/// <param name="Name">위젯의 이름</param>
	/// <returns>생성된 위젯</returns>
	template <typename T>
	T* CreateWidget(const std::string& Name)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->SetScene(m_Scene);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		// 현재 위젯을 바꾼다.
		m_Widget = Widget;

		return Widget;
	}
};

