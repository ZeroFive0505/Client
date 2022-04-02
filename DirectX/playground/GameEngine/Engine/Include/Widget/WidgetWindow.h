#pragma once

#include "../Ref.h"
#include "Widget.h"

/// <summary>
/// 여러 위젯을 가지는 기본 윈도우 클래스
/// 이 윈도우를 상속받아 다른 윈도우를 만들 수 있다.
/// </summary>
class CWidgetWindow :
	public CRef
{
	// 뷰포트뿐만 아니라 위젯 컴포넌트도 위젯 윈도우를 생성이 가능하게 한다.
	friend class CViewport;
	friend class CWidgetComponent;
	friend class CEngine;

protected:
	CWidgetWindow();
	CWidgetWindow(const CWidgetWindow& window);
	virtual ~CWidgetWindow();

protected:
	class CWidgetComponent* m_OwnerComponent;
	class CViewport* m_Viewport;
	int m_ZOrder;
	Vector2 m_Pos;
	Vector2 m_Size;
	bool m_Start;

	std::list<CSharedPtr<CWidget>> m_WidgetList;

public:
	class CWidgetComponent* GetWidgetComponent() const
	{
		return m_OwnerComponent;
	}

	inline class CViewport* GetViewport() const
	{
		return m_Viewport;
	}

	inline int GetWidgetCount() const
	{
		return (int)m_WidgetList.size();
	}

	inline Vector2 GetWindowPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetWindowSize() const
	{
		return m_Size;
	}

	inline int GetZOrder() const
	{
		return m_ZOrder;
	}

public:
	inline void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	inline void SetPos(const Vector2& pos)
	{
		m_Pos = pos;
	}

	inline void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	inline void SetSize(const Vector2& size)
	{
		m_Size = size;
	}

	inline void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual bool CollisionMouse(const Vector2& mousePos);
	virtual CWidgetWindow* Clone();

private:
	static bool SortWidget(const CSharedPtr<CWidget> src, const CSharedPtr<CWidget> dest);

public:
	template <typename T>
	T* FindWidget(const std::string& name)
	{
		auto iter = m_WidgetList.begin();
		auto iterEnd = m_WidgetList.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetName() == name)
				return (T*)*(*iter);
		}

		return nullptr;
	}

	template <typename T>
	T* CreateWidget(const std::string& name)
	{
		T* widget = new T;

		widget->SetName(name);
		widget->SetOwner(this);

		if (!widget->Init())
		{
			SAFE_RELEASE(widget);
			return nullptr;
		}

		m_WidgetList.push_back((CWidget*)widget);

		return widget;
	}
};

