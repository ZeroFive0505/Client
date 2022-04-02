#pragma once

#include "../Ref.h"

class CUIWidget :
	public CRef
{
public:
	CUIWidget();
	CUIWidget(const CUIWidget& widget);
	virtual ~CUIWidget();

protected:
	class CScene* m_Scene;
	int m_Zorder;
	bool m_Visibility;
	class CUIWindow* m_Owner;
	Vector2 m_Pos;
	Vector2 m_Size;

public:
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

	inline void SetOwner(class CUIWindow* owner)
	{
		m_Owner = owner;
	}

	inline void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

	inline void SetVisibility(bool visibility)
	{
		m_Visibility = visibility;
	}

	inline bool GetVisibility() const
	{
		return m_Visibility;
	}

	inline void SetZOrder(int ZOrder)
	{
		m_Zorder = ZOrder;
	}

	inline int GetZOrder() const
	{
		return m_Zorder;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CUIWidget* Clone();
};

