#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "../Object/Line.h"


class CPendulum :
	public CGameObject
{
	friend class CScene;
public:
	CPendulum();
	CPendulum(const CPendulum& obj);
	virtual ~CPendulum();

private:
	CSharedPtr<CLine> m_Line;
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderCircle> m_Body;

	Vector2 m_Origin;
	Vector2 m_Pendulum;

	float m_ArmLength;
	float m_Angle;
	float m_AngularVelocity;
	float m_AngularAcceleration;
	float m_Damping;

	float m_Gravity;

	bool m_OnMouse;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CPendulum* Clone();

public:
	inline void SetArmLength(float length)
	{
		m_ArmLength = length;
		m_Line->SetWorldScale(length, 1.0f, 1.0f);
	}

	inline void SetAngle(float angle)
	{
		m_Angle = angle;
	}

	inline void SetAngularVelocity(float aVel)
	{
		m_AngularVelocity = aVel;
	}

	inline void SetGravity(float gravity)
	{
		m_Gravity = gravity;
	}

public:
	void OnMouseCollision(const sCollisionResult& result)
	{
		m_OnMouse = true;
	}

	void OnMouseCollisionExit(const sCollisionResult& result)
	{
		m_OnMouse = false;
	}
};

