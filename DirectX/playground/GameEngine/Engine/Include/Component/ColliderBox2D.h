#pragma once

#include "ColliderComponent.h"

/// <summary>
/// 사각형 형태의 충돌체
/// </summary>
class CColliderBox2D :
	public CColliderComponent
{
	friend class CGameObject;

protected:
	CColliderBox2D();
	CColliderBox2D(const CColliderBox2D& com);
	virtual ~CColliderBox2D();

protected:
	sBox2DInfo m_Info;

public:
	inline sBox2DInfo GetInfo() const
	{
		return m_Info;
	}

	void SetExtent(float width, float height)
	{
		m_Info.length.x = width;
		m_Info.length.y = height;

		// 반 길이를 2배 늘려서 사각형을 만든다.
		SetWorldScale(m_Info.length.x * 2.0f, m_Info.length.y * 2.0f, 1.0f);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderBox2D* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual bool Collision(CColliderComponent* dest);
	virtual bool CollisionMouse(const Vector2& mousePos);
};

