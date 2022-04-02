#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Input.h"

class CMover :
	public CGameObject
{
	friend class CScene;
protected:
	CMover();
	CMover(const CMover& obj);
	virtual ~CMover();

private:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderCircle> m_Collider;

private:
	Vector2 m_ScreenSize;

	Vector2 m_Velocity;
	Vector2 m_Acceleration;
	float m_Mass;



	Vector2 m_CenterPos;
	Vector2 m_Amplitude;
	Vector2 m_AngularVelocity;
	float m_AngularAcceleration;
	Vector2 m_Angle;

	bool m_OnMouse;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMover* Clone();

public:
	inline void SetMass(float mass)
	{
		m_Mass = mass;

		m_Sprite->SetWorldScale(m_Mass * 5.0f, m_Mass * 5.0f, 1.0f);
		m_Collider->SetInfo(Vector2(0.0f, 0.0f), m_Mass * 2.5f);
	}

	inline void SetAngle(const Vector2& angle)
	{
		m_Angle = angle;
	}

	inline void SetCenterPos(const Vector2& pos)
	{
		m_CenterPos = pos;
	}

	inline void SetAmplitude(const Vector2& amplitude)
	{
		m_Amplitude = amplitude;
	}

	inline void SetVelocity(const Vector2& velocity)
	{
		m_Velocity = velocity;
	}

public:
	inline float GetMass() const
	{
		return m_Mass;
	}

	inline Vector2 GetAnlge() const
	{
		return m_Angle;
	}

	inline Vector2 GetCenterPos() const
	{
		return m_CenterPos;
	}

	inline Vector2 GetAcceleration() const
	{
		return m_Acceleration;
	}

	inline Vector2 GetVelocity() const
	{
		return m_Velocity;
	}

	inline float GetAngularAcceleration() const
	{
		return m_AngularAcceleration;
	}

	inline Vector2 GetAngularVelocity() const
	{
		return m_AngularVelocity;
	}

	inline Vector2 GetAmplitude() const
	{
		return m_Amplitude;
	}

	inline bool IsOnMouse() const
	{
		return m_OnMouse && CInput::GetInst()->GetMouseLButtonClick();
	}

public:
	void ApplyForce(const Vector2& force);
	void ApplyForce(float x, float y);
	void AddAngularForceX(float force);
	void AddAngularForceY(float force);
	void EdgeCheck();

private:
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);

private:
	void OnMouseCollisionEnter(const sCollisionResult& result);
	void OnMouseCollisionExit(const sCollisionResult& result);
};

