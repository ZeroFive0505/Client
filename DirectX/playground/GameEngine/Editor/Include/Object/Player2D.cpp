#include "Player2D.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Resource/Material/Material.h"
#include "Animation/AnimationSequence2DInstance.h"

CPlayer2D::CPlayer2D()
{
	SetTypeID<CPlayer2D>();
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSprite");
}

CPlayer2D::~CPlayer2D()
{
}

void CPlayer2D::Start()
{
	CGameObject::Start();

	CInput::GetInst()->SetCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveLeft", KeyState_Push, this, &CPlayer2D::MoveLeft);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveRight", KeyState_Push, this, &CPlayer2D::MoveRight);
}

bool CPlayer2D::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2DInstance* anim = m_Sprite->GetAnimationInstance();

	// 미리 저장되어있는 시퀀스 파일들을 불러온다.
	anim->AddAnimation(TEXT("Idle.sqc"), ANIMATION_PATH, "Idle", true);
	anim->AddAnimation(TEXT("Walk.sqc"), ANIMATION_PATH, "Run", true);

	anim->Play();

	m_Sprite->SetRelativeScale(100.0f, 100.0f, 1.0f);
	m_Sprite->SetRelativePos(100.0f, 50.0f, 0.0f);
	m_Sprite->SetPivot(0.5f, 0.0f, 0.0f);

	return true;
}

void CPlayer2D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CPlayer2D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CPlayer2D::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSprite");
}

void CPlayer2D::MoveUp(float deltaTime)
{

}

void CPlayer2D::MoveDown(float deltaTime)
{

}

void CPlayer2D::MoveLeft(float dletaTime)
{

}

void CPlayer2D::MoveRight(float deltaTime)
{

}