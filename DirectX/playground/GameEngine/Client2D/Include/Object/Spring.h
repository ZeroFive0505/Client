#pragma once

#include "GameObject/GameObject.h"
#include "Component/StaticMeshComponent.h"
#include "Mover.h"

class CSpring :
	public CGameObject
{
	friend class CScene;

protected:
	CSpring();
	CSpring(const CSpring& obj);
	virtual ~CSpring();

private:
	CSharedPtr<CStaticMeshComponent> m_Spring;
	CSharedPtr<CMover> m_Mover;

	Vector2 m_Anchor;

	float m_RestLength;
	float m_SpringConstant;

	float m_AngularVelocity;

	float m_Damping;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSpring* Clone();

public:
	inline void SetAnchor(const Vector2& anchor)
	{
		m_Anchor = anchor;
		SetWorldPos(m_Anchor.x, m_Anchor.y, 0.0f);
	}

	inline void SetRestLength(float length)
	{
		m_RestLength = length;
		SetWorldScale(m_RestLength, 1.0f, 1.0f);
	}


	inline void SetSpringConstant(float springConstant)
	{
		m_SpringConstant = springConstant;
	}

	inline void ConnectMover(CMover* mover) 
	{
		m_Mover = mover;
	}

private:
	void ConstrainLength();
};

