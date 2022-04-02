#pragma once

#include "SceneComponent.h"
#include "../Widget/WidgetWindow.h"
#include "../Scene/Scene.h"

/// <summary>
/// ������ ��ũ�� �Ǵ� ���� ���������� ��ġ���� UI�� ������ ���ִ� ������Ʈ
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
	// �� ���� ������Ʈ�� ���� �����츦 ������ ���� �� �ִ�.
	// �� ������� ����Ʈ���� ������ �Ǵ°��� �ƴ϶� �� ������Ʈ���� ���� ó���� �Ѵ�.
	CSharedPtr<CWidgetWindow> m_WidgetWindow;
	// ��� ȭ�鿡 �������Ǵ���
	WidgetComponent_Space m_Space;

public:
	void SetWidgetSpace(WidgetComponent_Space space)
	{
		m_Space = space;

		if (m_Space == WidgetComponent_Space::Screen)
		{
			// ��ũ�� �������� �������� ���� ȸ���� ���� �ʴ´�.
			SetInheritRotX(false);
			SetInheritRotY(false);
			SetInheritRotZ(false);
			// ��ũ���������� �������� ���� �ٸ� ���̾ �̿���
			// ��� ���� ������Ʈ�� �������� ���Ŀ� �������ǰ� �Ѵ�.
			SetLayerName("ScreenWidgetComponent");
		}
		else
		{
			SetInheritRotX(true);
			SetInheritRotY(true);
			SetInheritRotZ(true);
			// ��ũ�� ������ �ƴ� ���� ���� ���������� ������
			// ���� ȸ���� ��ӹް� �ٸ� ���� ������Ʈ�� ���� ���̾ �̿��ؼ� �������Ѵ�.
			SetLayerName("Default");
		}
	}

	inline WidgetComponent_Space GetWidgetSpace() const
	{
		return m_Space;
	}

	// �� ���� ������Ʈ�� �������ִ� ������ �����Ѵ�.
	inline void SetWidgetWindow(CWidgetWindow* window)
	{
		// ���� ������ �ִ� ���� �����찡 �־��ٸ�
		// �� ������������ �����ڸ� ���ش�.
		if (m_WidgetWindow)
			m_WidgetWindow->m_OwnerComponent = nullptr;

		// ���Ӱ� �Ҵ�
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
	// ���� ������Ʈ�� ���� ���� �����츦 �����Ѵ�.
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

