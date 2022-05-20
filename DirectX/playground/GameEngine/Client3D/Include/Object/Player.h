#pragma once

#include "GameObject/GameObject.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ArmComponent.h"
#include "Component/LightComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"

class CPlayer :
	public CGameObject
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

private:
	CSharedPtr<CAnimationMeshComponent> m_Mesh;
	// CSharedPtr<CColliderBox3D> m_Collider;
	CSharedPtr<CArmComponent> m_Arm;
	CSharedPtr<CCameraComponent> m_Camera;
	// CSharedPtr<CLightComponent> m_FlashLight;
	class CPlayerAnimation* m_Animation;
	Vector3 m_Velocity;
	CSharedPtr<class CWeapon> m_Weapon;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CPlayer* Clone();

private:
	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void YRotation(float deltaTime);
	void InvYRotation(float deltaTime);
	void Attack(float deltaTime);
};

