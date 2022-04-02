
#include "Player.h"
#include "../Input.h"
#include "../Scene/Scene.h"
#include "../GameManager.h"
#include "../Collision//ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/ProgressBar.h"
#include "../UI/UIText.h"

CPlayer::CPlayer() :
	m_Skill1Enable(false),
	m_Skill1Time(0.0f),
	fElapsedTime(0.0f)
{
}

CPlayer::CPlayer(const CPlayer& obj)	:
	CCharacter(obj)
{
	m_Skill1Time = obj.m_Skill1Time;
	m_Skill1Enable = false;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	// InputŬ������ �ݹ� ���ε� �Լ��� ȣ���Ͽ� �ݹ��Լ��� ���´�.
	CInput::GetInst()->SetCallback<CPlayer>("MoveUp", KeyState_Push, this, &CPlayer::MoveUp);

	CInput::GetInst()->SetCallback<CPlayer>("MoveDown", KeyState_Push, this, &CPlayer::MoveDown);

	CInput::GetInst()->SetCallback<CPlayer>("MoveLeft", KeyState_Push, this, &CPlayer::MoveLeft);

	CInput::GetInst()->SetCallback<CPlayer>("MoveRight", KeyState_Push, this, &CPlayer::MoveRight);

	CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Up, this, &CPlayer::BulletFire);

	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down, this, &CPlayer::Pause);

	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down, this, &CPlayer::Resume);

	CInput::GetInst()->SetCallback<CPlayer>("Skill1", KeyState_Down, this, &CPlayer::Skill1);

	CInput::GetInst()->SetCallback<CPlayer>("Jump", KeyState_Down, this, &CPlayer::JumpKey);
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;
	// �߽��� �Ǵ� ������ ��´�.
	SetPivot(0.5f, 1.0f);
	// �ִϸ��̼��� �����Ѵ�.
	// ���� ������Ʈ�� �ִϸ��̼� Ŭ���� ������ Ÿ���� ������ �ִ�.
	CreateAnimation();
	// ���� ���� �����ҽ��� �ִ� �ִϸ��̼��� �÷��̾�� �߰��Ѵ�.
	AddAnimation("PlayerRightIdle");
	AddAnimation("PlayerRightWalk", true, 0.6f);
	AddAnimation("PlayerRightAttack", false, 0.5f);
	AddAnimation("PlayerRightSkill1", false, 0.5f);

	AddAnimationNotify<CPlayer>("PlayerRightAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("PlayerRightAttack", this, &CPlayer::AttackEnd);

	AddAnimationNotify<CPlayer>("PlayerRightSkill1", 2, this, &CPlayer::Skill1Enable);
	SetAnimationEndNotify<CPlayer>("PlayerRightSkill1", this, &CPlayer::Skill1End);

	/*CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 75.f);
	Body->SetOffset(0.f, -37.5f);*/

	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	//Head->SetExtent(40.f, 30.f);
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

	m_HPBarWidget = CreateWidgetComponent("HPBarWidget");

	CProgressBar* HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");

	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));

	m_HPBarWidget->SetPos(-25.0f, -120.0f);

	m_MPBarWidget = CreateWidgetComponent("MPBarWidget");

	CProgressBar* MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");

	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));

	m_MPBarWidget->SetPos(-25.0f, -100.0f);

	CWidgetComponent* NameWidget = CreateWidgetComponent("NameWidget");

	CUIText* NameText = NameWidget->CreateWidget<CUIText>("NameText");

	NameText->SetText(TEXT("PK ������"));
	NameText->SetTextColor(255, 0, 0);

	NameWidget->SetPos(-25.0f, -150.0f);

	m_CharacterInfo.HP = 500;
	m_CharacterInfo.HPMax = 500;

	m_CharacterInfo.MP = 100;
	m_CharacterInfo.MPMax = 100;

	SetPhysicsSimulate(true);
	SetJumpVelocity(100.0f);
	SetSideWallCheck(true);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (GetAsyncKeyState(VK_F1) & 0x8000)
		SetAttackSpeed(1.5f);

	if (m_Skill1Enable)
	{
		m_Skill1Time += DeltaTime;

		if (m_Skill1Time >= 3.0f)
		{
			m_Skill1Enable = false;
			m_Skill1Time = 0.0f;

			SetTimeScale(1.0f);
			CGameManager::GetInst()->SetTimeScale(1.0f);

		}
	}

	if (fElapsedTime >= 0.5f)
	{
		MPRegen();
		fElapsedTime -= 0.5f;
	}

	fElapsedTime += DeltaTime;

	// ���ݸ�� ��������Ʈ ��Ʈ �Ʒ��� ������ �����Ƿ� 20�ȼ� �Ʒ��� ������. (���� ��������)
	if (CheckCurrentAnimation("PlayerRightAttack"))
		SetOffset(0.0f, 20.0f);
	else
		SetOffset(0.0f, 0.0f);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	if (CheckCurrentAnimation("PlayerRightWalk") && m_Velocity.Length() <= 0.0f)
	{
		ChangeAnimation("PlayerRightIdle");
	}
}

void CPlayer::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayer::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

float CPlayer::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);

	// ���� ������ ĳ���� ����â�� �����´�.
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	if (State)
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	CProgressBar* HPBar = (CProgressBar*)m_HPBarWidget->GetWidget();

	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	return Damage;
}

float CPlayer::SetMP(float MP)
{
	MP = CCharacter::SetMP(MP);

	CProgressBar* MPBar = (CProgressBar*)m_MPBarWidget->GetWidget();

	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	return MP;
}

void CPlayer::MPRegen()
{
	if (m_CharacterInfo.MP >= m_CharacterInfo.MPMax)
	{
		m_CharacterInfo.MP = m_CharacterInfo.MPMax;
		return;
	}

	m_CharacterInfo.MP += 1;

	CProgressBar* MPBar = (CProgressBar*)m_MPBarWidget->GetWidget();

	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
}

void CPlayer::MoveUp(float DeltaTime)
{
	Move(Vector2(0.0f, -1.0f));
	ChangeAnimation("PlayerRightWalk");
}

void CPlayer::MoveDown(float DeltaTime)
{
	Move(Vector2(0.0f, 1.0f));
	ChangeAnimation("PlayerRightWalk");
}

void CPlayer::MoveLeft(float DeltaTime)
{
	Move(Vector2(-1.0f, 0.0f));
	ChangeAnimation("PlayerRightWalk");
}

void CPlayer::MoveRight(float DeltaTime)
{
	Move(Vector2(1.0f, 0.0f));
	ChangeAnimation("PlayerRightWalk");
}

void CPlayer::BulletFire(float DeltaTime)
{
	ChangeAnimation("PlayerRightAttack");

}

// �����ÿ��� ���� �Ŵ������� �ð� ������ ������� 0���� �����.
void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.0f);
}

// �ٽ� ������ �����Ҷ��� ������ 1�� �����.
void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.0f);
}

void CPlayer::Skill1(float DeltaTime)
{
	if (m_CharacterInfo.MP < 50)
		return;

	ChangeAnimation("PlayerRightSkill1");
}

void CPlayer::AttackEnd()
{
	ChangeAnimation("PlayerRightIdle");
}

void CPlayer::JumpKey(float DeltaTime)
{
	Jump();
}

void CPlayer::Fire()
{
	// ���� ���� ���ο� ������Ʈ�� �����.
	// �÷��̾��� ����ü�� ��� ������Ÿ������ �̹� ���� ���� �ε��ɶ� ����Ǿ� �ִ� �װ� �����ؼ� ����.
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet", "PlayerBullet",
		Vector2(m_Pos + Vector2(75.0f, 0.0f)),
		Vector2(50.0f, 50.0f));
}

void CPlayer::Skill1End()
{
	ChangeAnimation("PlayerRightIdle");
}

void CPlayer::Skill1Enable()
{
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.0f);
	m_Skill1Enable = true;
	SetMP((float)(m_CharacterInfo.MP - 50));
}
