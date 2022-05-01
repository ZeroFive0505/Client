#include "Player.h"
#include "PlayerAnimation.h"
#include "Input.h"

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Arm = (CArmComponent*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Arm = CreateComponent<CArmComponent>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_Mesh->AddChild(m_Arm);

	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("PlayerMesh");
	m_Mesh->CreateAnimationInstance<CPlayerAnimation>();

	m_Animation = (CPlayerAnimation*)m_Mesh->GetAnimationInstance();

	m_Mesh->SetRelativeScale(0.05f, 0.05f, 0.05f);

	m_Camera->SetRelativePos(0.0f, 0.0f, -5.0f);

	m_Arm->SetOffset(0.0f, 1.0f, 0.0f);
	m_Arm->SetRelativeRotation(15.0f, 0.0f, 0.0f);
	m_Arm->SetTargetDistance(5.0f);

	CInput::GetInst()->SetCallback<CPlayer>("Attack1", KeyState_Down, this, &CPlayer::Attack);
	CInput::GetInst()->SetCallback<CPlayer>("MoveForward", KeyState_Push, this, &CPlayer::MoveForward);


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
	m_Velocity += GetWorldAxis(AXIS_Z) * 1.0f * deltaTime;

	AddWorldPos(m_Velocity);
}

void CPlayer::MoveBackward(float deltaTime)
{
}

void CPlayer::YRotation(float deltaTime)
{
}

void CPlayer::InvYRotation(float deltaTime)
{
}

void CPlayer::Attack(float deltaTime)
{
	m_Animation->ChangeAnimation("Attack");
	m_Animation->SetIdleEnable(false);
}
