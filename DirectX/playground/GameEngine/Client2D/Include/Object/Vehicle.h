#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/StaticMeshComponent.h"
#include "Line.h"
#include "Circle.h"

class CVehicle :
	public CGameObject
{
	friend class CScene;

protected:
	CVehicle();
	CVehicle(const CVehicle& obj);
	virtual ~CVehicle();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CVehicle* Clone();

private:
	CSharedPtr<CSpriteComponent> m_Sprite;

private:
	class CFlowField* m_Field;
	class CPath* m_Path;

	Vector2 m_Velocity;
	Vector2 m_Acceleration;
	Vector2 m_Resolution;
	float m_MaxDist;
	float m_MaxSpeed;
	float m_MaxForce;
	float m_Mass;

public:
	inline void SetFlowField(class CFlowField* field)
	{
		m_Field = field;
	}

	inline void SetPath(class CPath* path)
	{
		m_Path = path;
	}

public:
	void Seek(const Vector2& target, float deltaTime);
	void Flee(const Vector2& target, float deltaTime);
	void Arrive(const Vector2& target, float deltaTime);
	void Follow();
	void PathFollow(float deltaTime);
	void ApplyForce(const Vector2& force);
	void ApplyForce(float x, float y);
	void CheckEdge();
};

