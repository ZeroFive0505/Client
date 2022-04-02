#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"


class CAttractor :
	public CGameObject
{
	friend class CScene;
protected:
	CAttractor();
	CAttractor(const CAttractor& obj);
	virtual ~CAttractor();

private:
	float m_Mass;
	CSharedPtr<CSpriteComponent> m_Sprite;

	Vector2 m_ScreenSize;

	float m_G;

private:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CAttractor* Clone();

public:
	void Attract(CGameObject* obj, float deltaTime);

public:
	inline void SetUniversalGravitation(float g)
	{
		m_G = g;
	}

	inline void SetMass(float mass)
	{
		m_Mass = mass;
		m_Sprite->SetWorldScale(m_Mass * 25.0f, m_Mass * 25.0f, 1.0f);
	}
};

