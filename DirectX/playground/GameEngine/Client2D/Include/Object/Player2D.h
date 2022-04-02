#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"

class CPlayer2D :
	public CGameObject
{
	friend class CScene;

protected:
	CPlayer2D();
	CPlayer2D(const CPlayer2D& obj);
	virtual ~CPlayer2D();

private:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CSpriteComponent> m_LeftChildSprite;
	CSharedPtr<CSpriteComponent> m_RightChildSprite;
	CSharedPtr<CSceneComponent> m_LeftChildMuzzle;
	CSharedPtr<CSceneComponent> m_RightChildMuzzle;
	CSharedPtr<CSceneComponent> m_RootChild;
	CSharedPtr<CSceneComponent> m_Muzzle;

	CSharedPtr<CSpriteComponent> m_OrbitChild1;
	CSharedPtr<CSpriteComponent> m_OrbitChild2;
	CSharedPtr<CSpriteComponent> m_OrbitChild3;
	CSharedPtr<CSpriteComponent> m_OrbitChild4;

	CSharedPtr<CColliderBox2D> m_Body;
	CSharedPtr<CCameraComponent> m_Camera;

	CSharedPtr<CWidgetComponent> m_SimpleHUDWidget;

	float m_Opacity;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CPlayer2D* Clone();

private:
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void RotationZInv(float deltaTime);
	void RotationZ(float deltaTime);
	void Attack(float deltaTime);
	void Attack1(float deltaTime);
	void BackToIdle(float deltaTime);
	void Skill(float deitlaTime);
};

