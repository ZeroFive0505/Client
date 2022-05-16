#pragma once

#include "ColliderComponent.h"

class CColliderBox3D :
	public CColliderComponent
{
	friend class CGameObject;

protected:
	CColliderBox3D();
	CColliderBox3D(const CColliderBox3D& com);
	virtual ~CColliderBox3D();

protected:
	sBox3DInfo m_Info;

public:
	inline sBox3DInfo GetInfo() const
	{
		return m_Info;
	}

	void SetExtent(float width, float height, float depth)
	{
		m_Info.length.x = width;
		m_Info.length.y = height;
		m_Info.length.z = depth;

		SetWorldScale(m_Info.length.x * 2.0f, m_Info.length.y * 2.0f, m_Info.length.z * 2.0f);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderBox3D* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual bool Collision(CColliderComponent* dest);
	virtual bool CollisionMouse(const Vector2& mousePos);
};

