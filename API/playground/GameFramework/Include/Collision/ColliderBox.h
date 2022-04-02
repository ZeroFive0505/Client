#pragma once

#include "Collider.h"

/// <summary>
/// 기본 Collider 타입 박스 타입 클래스
/// </summary>
class CColliderBox :
	public CCollider
{
	friend class CGameObject;

protected:
	CColliderBox();
	CColliderBox(const CColliderBox& collider);
	virtual ~CColliderBox();

protected:
	float m_Width;
	float m_Height;
	RectInfo m_Info;

public:
	void SetExtent(float Width, float Height)
	{
		m_Width = Width;
		m_Height = Height;
	}

	RectInfo GetInfo() const
	{
		return m_Info;
	}

	virtual float GetBottom() const
	{
		return m_Info.Bottom;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CColliderBox* Clone();
	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionMouse(const Vector2& MousePos);
};

