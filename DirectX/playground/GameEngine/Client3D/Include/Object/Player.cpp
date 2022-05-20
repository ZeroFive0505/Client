#include "Player.h"
#include "PlayerAnimation.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Weapon.h"

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Player");
	m_Arm = (CArmComponent*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	// m_FlashLight = (CLightComponent*)FindComponent("Flash Light");
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Player");
	m_Arm = CreateComponent<CArmComponent>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	// m_Collider = CreateComponent<CColliderBox3D>("Collider");
	// m_FlashLight = CreateComponent<CLightComponent>("Flash Light");

	m_Mesh->AddChild(m_Arm);
	// m_Mesh->AddChild(m_Collider);
	m_Arm->AddChild(m_Camera);
	// m_Arm->AddChild(m_FlashLight);

	// m_Collider->SetExtent(5.0f, 5.0f, 5.0f);
	// m_Collider->SetCollisionProfile("Player");
	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	// m_FlashLight->SetInheritRotX(true);
	// m_FlashLight->SetInheritRotY(true);
	// m_FlashLight->SetInheritRotZ(true);
	// 
	// m_FlashLight->SetLightType(Light_Type::Spot);
	// 
	// m_FlashLight->SetDistance(20.0f);
	// m_FlashLight->SetAttConst3(0.1f);

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("PlayerMesh");
	m_Mesh->CreateAnimationInstance<CPlayerAnimation>();

	m_Animation = (CPlayerAnimation*)m_Mesh->GetAnimationInstance();

	m_Mesh->SetRelativeScale(0.02f, 0.02f, 0.02f);

	m_Camera->SetRelativePos(0.0f, 0.0f, -5.0f);

	m_Arm->SetOffset(0.0f, 1.0f, 0.0f);
	m_Arm->SetRelativeRotation(45.0f, 0.0f, 0.0f);
	m_Arm->SetTargetDistance(5.0f);

	m_Weapon = m_Scene->CreateGameObject<CWeapon>("Weapon");

	m_Mesh->AddChild(m_Weapon, "Weapon");

	CInput::GetInst()->SetCallback<CPlayer>("Attack", KeyState_Down, this, &CPlayer::Attack);
	CInput::GetInst()->SetCallback<CPlayer>("MoveForward", KeyState_Push, this, &CPlayer::MoveForward);
	CInput::GetInst()->SetCallback<CPlayer>("RotationY", KeyState_Push, this, &CPlayer::YRotation);
	CInput::GetInst()->SetCallback<CPlayer>("RotationYInv", KeyState_Push, this, &CPlayer::InvYRotation);


	return true;
}

void CPlayer::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	// m_Arm->AddRelativeRotationY(90.0f * deltaTime);

	if (CInput::GetInst()->GetWheelDirection())
	{
		float dist = m_Arm->GetTargetDistance() - CInput::GetInst()->GetWheelDirection() * 0.1f;

		m_Arm->SetTargetDistance(dist);
	}

	if (m_Velocity.Length() > 0.0f)
	{
		m_Animation->ChangeAnimation("Walk");
		m_Animation->SetIdleEnable(true);
	}
	else if (m_Animation->GetIdleEnable())
	{
		m_Animation->ChangeAnimation("Idle");
	}
}

void CPlayer::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);

	m_Velocity = Vector3::Zero;
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::MoveForward(float deltaTime)
{
	m_Velocity += GetWorldAxis(AXIS_Z) * 10.0f * deltaTime;

	AddWorldPos(m_Velocity);
}

void CPlayer::MoveBackward(float deltaTime)
{
}

void CPlayer::YRotation(float deltaTime)
{
	m_Arm->AddWorldRotationY(180.0f * deltaTime);
}

void CPlayer::InvYRotation(float deltaTime)
{
	m_Arm->AddWorldRotationY(-180.0f * deltaTime);
}

void CPlayer::Attack(float deltaTime)
{
	m_Animation->ChangeAnimation("Attack");
	m_Animation->SetIdleEnable(false);
}
