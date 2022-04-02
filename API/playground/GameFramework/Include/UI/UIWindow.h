#pragma once

#include "../Ref.h"
#include "UIWidget.h"


/// <summary>
/// 윈도우 클래스이다. 윈도우에는 여러가지 위젯이 들어 갈 수 있다.
/// </summary>
class CUIWindow :
	public CRef
{
public:
	CUIWindow();
	CUIWindow(const CUIWindow& widget);
	virtual ~CUIWindow();

protected:
	// 현재 어떤 씬에 속해있는지.
	class CScene* m_Scene;
	// Z값은 어떻게 되는지.
	int m_ZOrder;

	// 가지고 있는 위젯
	CUIWidget** m_WidgetArray;
	int m_WidgetCount;
	int m_WidgetCapacity;

	// 보이는 여부
	bool m_Visibility;
	Vector2 m_Pos;
	Vector2 m_Size;

public:
	CUIWidget* GetWidget(int Index)
	{
		return m_WidgetArray[Index];
	}

	int GetWidgetCount() const
	{
		return m_WidgetCount;
	}

	Vector2 GetPos() const
	{
		return m_Pos;
	}

	Vector2 GetSize() const
	{
		return m_Size;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	int GetZOrder() const
	{
		return m_ZOrder;
	}

	void SetVisibility(bool Visibility)
	{
		m_Visibility = Visibility;
	}

	bool GetVisibility() const
	{
		return m_Visibility;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);

public:
	static int SortZOrder(const void* Src, const void* Dest);

public:
	/// <summary>
	/// 현재 윈도우에 속해있는 위젯에서 원하는 위젯을 검색한다.
	/// </summary>
	/// <typeparam name="T">찾고자 하는 위젯의 타입</typeparam>
	/// <param name="Name">위젯의 이름</param>
	/// <returns>찾은 위젯</returns>
	template <typename T>
	T* FindWidget(const std::string& Name)
	{
		for (int i = 0; i < m_WidgetCount; i++)
		{
			if (m_WidgetArray[i]->GetName() == Name)
				return (T*)m_WidgetArray[i];
		}

		return nullptr;
	}

public:
	/// <summary>
	/// 현재 윈도우에 새로운 위젯을 추가한다.
	/// </summary>
	/// <typeparam name="T">추가하고자 하는 위젯의 타입</typeparam>
	/// <param name="Name">위젯의 이름</param>
	/// <returns>생성된 위젯</returns>
	template <typename T>
	T* CreateWidget(const std::string& Name)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->SetScene(m_Scene);
		Widget->SetOwner(this);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		if (m_WidgetCount == m_WidgetCapacity)
		{
			m_WidgetCapacity *= 2;

			CUIWidget** Array = new CUIWidget * [m_WidgetCapacity];

			memcpy(Array, m_WidgetArray, sizeof(CUIWidget*) * m_WidgetCount);

			SAFE_DELETE_ARRAY(m_WidgetArray);

			m_WidgetArray = Array;
		}

		Widget->AddRef();
		m_WidgetArray[m_WidgetCount++] = Widget;

		return (T*)Widget;
	}
};

