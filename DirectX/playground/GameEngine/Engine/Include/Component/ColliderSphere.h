#pragma once

#include "ColliderComponent.h"

class CColliderSphere :
	public CColliderComponent
{
	friend class CGameObject;

protected:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& com);
	virtual ~CColliderSphere();

protected:
	sSphereInfo m_Info;

public:
	inline sSphereInfo GetInfo() const
	{
		return m_Info;
	}

	inline void SetInfo(const Vector3& center, float radius)
	{
		m_Info.center = center;

		m_Info.radius = radius;

		SetWorldScale(m_Info.radius * 2.0f, m_Info.radius * 2.0f, m_Info.radius * 2.0f);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderSphere* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual bool Collision(CColliderComponent* dest);
	virtual bool CollisionMouse(const Vector2& mousePos);
	virtual bool CollisionRay(const sRay& ray);
};

