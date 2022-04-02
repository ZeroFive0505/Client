#pragma once

#include "SceneComponent.h"
#include "../Widget/WidgetWindow.h"
#include "../Scene/Scene.h"

/// <summary>
/// 씬에서 스크린 또는 월드 공간에서의 위치에서 UI를 렌더링 해주는 컴포넌트
/// </summary>
class CWidgetComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CWidgetComponent();
	CWidgetComponent(const CWidgetComponent& com);
	virtual ~CWidgetComponent();

protected:
	// 이 위젯 컴포넌트는 위젯 윈도우를 가지고 있을 수 있다.
	// 이 윈도우는 뷰포트에서 렌더링 되는것이 아니라 이 컴포넌트에서 렌더 처리를 한다.
	CSharedPtr<CWidgetWindow> m_WidgetWindow;
	// 어떻게 화면에 렌더링되는지
	WidgetComponent_Space m_Space;

public:
	void SetWidgetSpace(WidgetComponent_Space space)
	{
		m_Space = space;

		if (m_Space == WidgetComponent_Space::Screen)
		{
			// 스크린 공간에서 렌더링될 경우는 회전을 하지 않는다.
			SetInheritRotX(false);
			SetInheritRotY(false);
			SetInheritRotZ(false);
			// 스크린공간에서 렌더링될 경우는 다른 레이어를 이용해
			// 모든 게임 오브젝트가 렌더링된 이후에 렌더링되게 한다.
			SetLayerName("ScreenWidgetComponent");
		}
		else
		{
			SetInheritRotX(true);
			SetInheritRotY(true);
			SetInheritRotZ(true);
			// 스크린 공간이 아닐 경우는 월드 공간에서의 렌더링
			// 따라서 회전을 상속받고 다른 게임 오브젝트와 같은 레이어를 이용해서 렌더링한다.
			SetLayerName("Default");
		}
	}

	inline WidgetComponent_Space GetWidgetSpace() const
	{
		return m_Space;
	}

	// 이 위젯 컴포넌트가 가지고있는 위젯을 설정한다.
	inline void SetWidgetWindow(CWidgetWindow* window)
	{
		// 만약 가지고 있는 위젯 윈도우가 있었다면
		// 그 위젯윈도우의 소유자를 없앤다.
		if (m_WidgetWindow)
			m_WidgetWindow->m_OwnerComponent = nullptr;

		// 새롭게 할당
		m_WidgetWindow = window;

		m_WidgetWindow->m_OwnerComponent = this;
	}

	inline CWidgetWindow* GetWidgetWindow() const
	{
		return m_WidgetWindow;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CWidgetComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	// 위젯 컴포넌트를 통해 위젯 윈도우를 생성한다.
	template <typename T>
	T* CreateWidgetWindow(const std::string& name)
	{
		T* window = new T;

		window->SetName(name);
		window->m_Viewport = m_Scene->GetViewport();
		window->m_OwnerComponent = this;

		if (!window->Init())
		{
			SAFE_RELEASE(window);
			return nullptr;
		}

		m_WidgetWindow = window;

		return window;
	}
};

