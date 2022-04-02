#pragma once

#include "../GameInfo.h"
#include "../Widget/WidgetWindow.h"

/// <summary>
/// UI Window�� �����ϴ� Ŭ����
/// ȭ�鿡 ��� ������Ʈ�� �׷����� �� Ŭ������ UI�� �׷������̴�.
/// </summary>
class CViewport
{
	friend class CScene;

protected:
	CViewport();
	~CViewport();

private:
	class CScene* m_Scene;

public:
	inline class CScene* GetScene() const
	{
		return m_Scene;
	}

private:
	std::list<CSharedPtr<CWidgetWindow>> m_WindowList;

public:
	void Start();
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	bool CollisionMouse();
	void Collision();
	void Render();

private:
	static bool SortWindow(const CSharedPtr<CWidgetWindow> src, const CSharedPtr<CWidgetWindow> dest);

public:
	template <typename T>
	T* FindWidgetWindow(const std::string& name)
	{
		auto iter = m_WindowList.begin();
		auto iterEnd = m_WindowList.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetName() == name)
				return (T*)*iter;
		}

		return nullptr;
	}

	template <typename T>
	T* CreateWidgetWindow(const std::string& name)
	{
		T* window = new T;

		window->SetName(name);
		window->m_Viewport = this;

		if (!window->Init())
		{
			SAFE_RELEASE(window);
			return nullptr;
		}

		m_WindowList.push_back((CWidgetWindow*)window);

		return window;
	}
};

