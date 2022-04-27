#include "Player.h"
#include "PlayerAnimation.h"

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

	m_Mesh->SetRelativeScale(0.05f, 0.05f, 0.05f);

	m_Camera->SetRelativePos(0.0f, 0.0f, -5.0f);

	m_Arm->SetOffset(0.0f, 1.0f, 0.0f);
	m_Arm->SetRelativeRotation(15.0f, 0.0f, 0.0f);
	m_Arm->SetTargetDistance(10.0f);

	return true;
}

void CPlayer::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	m_Arm->AddRelativeRotationY(90.0f * deltaTime);
}

void CPlayer::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::MoveForward(float deltaTime)
{
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
}
