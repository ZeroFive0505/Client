#pragma once

#include "../Ref.h"

/// <summary>
/// ������ȿ� ���ԵǴ� ����
/// </summary>
class CUIWidget :
	public CRef

{
public:
	CUIWidget();
	CUIWidget(const CUIWidget& widget);
	virtual ~CUIWidget();

protected:
	// ���� �����ִ� ��
	class CScene* m_Scene;
	// Z���� ��� �Ǵ���
	int m_ZOrder;

	// ���̴� ����
	bool m_Visibility;
	// � �����쿡 �����ִ���.
	class CUIWindow* m_Owner;
	Vector2 m_Pos;
	Vector2 m_Size;

	// ���콺�� ���� �ö� �ִ���
	bool m_MouseHoverd;

public:
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

	void SetOwner(class CUIWindow* Owner)
	{
		m_Owner = Owner;
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetVisibility(bool Visibility)
	{
		m_Visibility = Visibility;
	}

	bool GetVisibility() const
	{
		return m_Visibility;
	}

	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	int GetZOrder() const
	{
		return m_ZOrder;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CUIWidget* Clone();

public:
	bool CollisionMouse(const Vector2& MousePos, float DeltaTime);

public:
	virtual void CollisionMouseHoveredCallback(float DeltaTime);
	virtual void CollisionMouseReleaseCallback(float DeltaTime);
};
