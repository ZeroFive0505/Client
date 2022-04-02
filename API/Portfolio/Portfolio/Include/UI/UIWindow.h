#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow :
	public CRef
{
	friend class CScene;

protected:
	CUIWindow();
	virtual ~CUIWindow();

protected:
	class CScene* m_Scene;
	int m_ZOrder;
	CUIWidget** m_WidgetArray;
	int m_WidgetCount;
	int m_WidgetCapacity;

	bool m_Visibility;
	Vector2 m_Pos;
	Vector2 m_Size;

public:
	inline CUIWidget* GetWidget(int index)
	{
		return m_WidgetArray[index];
	}

	inline int GetWidgetCount() const
	{
		return m_WidgetCount;
	}

	inline Vector2 GetPos() const
	{
		return m_Pos;
	}

	inline Vector2 GetSize() const
	{
		return m_Size;
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

	inline void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

	inline void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	inline int GetZOrder() const
	{
		return m_ZOrder;
	}

	void SetVisibility(bool visibility)
	{
		m_Visibility = visibility;
	}

	bool GetVisibility() const
	{
		return m_Visibility;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);

public:
	static int SortZOrder(const void* src, const void* dest);

public:
	inline void SortWidget()
	{
		if (m_WidgetCount >= 2)
		{
			qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZOrder);
		}
	}

public:
	template <typename T>
	T* FindWidget(const std::string& name)
	{
		for (int i = 0; i < m_WidgetCount; i++)
		{
			if (m_WidgetArray[i]->GetName() == name)
				return (T*)m_WidgetArray[i];
		}

		return nullptr;
	}
	
public:
	template <typename T>
	T* CreateWidget(const std::string& name)
	{
		T* widget = new T;

		widget->SetName(name);
		widget->SetScene(m_Scene);
		widget->SetOwner(this);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}

		if (m_WidgetCount == m_WidgetCapacity)
		{
			m_WidgetCapacity *= 2;

			CUIWidget** arr = new CUIWidget * [m_WidgetCapacity];

			memcpy(arr, m_WidgetArray, sizeof(CUIWidget*) * m_WidgetCount);

			SAFE_DELETE_ARRAY(m_WidgetArray);

			m_WidgetArray = arr;
		}

		widget->AddRef();
		m_WidgetArray[m_WidgetCount] = widget;
		m_WidgetCount++;

		return (T*)widget;
	}
};

