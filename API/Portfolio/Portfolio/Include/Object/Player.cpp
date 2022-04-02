#include "Player.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Timer.h"

CPlayer::CPlayer()
{

}

CPlayer::CPlayer(const CPlayer& obj)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	CInput::GetInst()->SetCallback<CPlayer>("Up", KeyState_Hold,
		this, &CPlayer::Up);

	CInput::GetInst()->SetCallback<CPlayer>("Down", KeyState_Hold,
		this, &CPlayer::Down);

	CInput::GetInst()->SetCallback<CPlayer>("Left", KeyState_Hold,
		this, &CPlayer::Left);

	CInput::GetInst()->SetCallback<CPlayer>("Right", KeyState_Hold,
		this, &CPlayer::Right);
}

bool CPlayer::Init()
{
	if(!CCharacter::Init())
		return false;

	// SetPos(CGameManager::GetInst()->GetResolution().width / 2, CGameManager::GetInst()->GetResolution().height / 2);

	// SetPivot(0.5f, 0.5f);

	return true;
}

void CPlayer::Update(float deltaTime)
{
	CCharacter::Update(deltaTime);
}

void CPlayer::PostUpdate(float deltaTime)
{
	CCharacter::PostUpdate(deltaTime);
}

void CPlayer::Collision(float deltaTime)
{
	CCharacter::Collision(deltaTime);
}

void CPlayer::Render(HDC hDC)
{
	// CCharacter::Render(hDC);
}

CPlayer* CPlayer::Clone()
{
	return nullptr;
}

void CPlayer::Up(float deltaTime)
{
	Move(Vector2(cosf(m_Angle), sinf(m_Angle)), 0.2f);
}

void CPlayer::Down(float deltaTime)
{
	Move(Vector2(-cosf(m_Angle), -sinf(m_Angle)), 0.2f);
}

void CPlayer::Left(float deltaTime)
{
	m_Angle -= 1.0f * deltaTime;
}

void CPlayer::Right(float deltaTime)
{
	m_Angle += 1.0f * deltaTime;
}
