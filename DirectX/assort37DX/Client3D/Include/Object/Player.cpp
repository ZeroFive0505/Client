
#include "Player.h"
#include "PlayerAnimation.h"

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Arm = (CArm*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Arm = CreateComponent<CArm>("Arm");
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

	m_Arm->SetOffset(0.f, 1.f, 0.f);
	m_Arm->SetRelativeRotation(15.f, 0.f, 0.f);
	m_Arm->SetTargetDistance(10.f);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//m_Arm->AddRelativeRotationY(90.f * DeltaTime);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::MoveFront(float DeltaTime)
{
}

void CPlayer::MoveBack(float DeltaTime)
{
}

void CPlayer::RotationYInv(float DeltaTime)
{
}

void CPlayer::RotationY(float DeltaTime)
{
}

void CPlayer::Attack(float DeltaTime)
{
}
