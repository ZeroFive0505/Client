#include "RCGPlayer.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Resource/Material/Material.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Engine.h"
#include "Scene/SceneResource.h"
#include "RCGEnemy.h"
#include "SmokeParticle.h"
#include "Component/TileMapComponent.h"
#include "Component/ColliderComponent.h"
#include "../ClientManager.h"
#include "../Widget/RCGMainWidget.h"

CRCGPlayer::CRCGPlayer() :
	m_Transform(nullptr),
	m_Body(nullptr),
	m_Bottom(nullptr),
	m_Sprite(nullptr),
	m_HitBox(nullptr),
	m_StateEnd{},
	m_MainWidget(nullptr)
{
	SetTypeID<CRCGPlayer>();
	m_Opacity = 1.0f;
	m_FacingRight = true;
}

CRCGPlayer::CRCGPlayer(const CRCGPlayer& obj) :
	CGameObject(obj)
{
	m_Transform = (CSceneComponent*)FindComponent("RCGPlayerTransform");
	m_Sprite = (CSpriteComponent*)FindComponent("RCGPlayerSprite");
	m_Body = (CColliderBox2D*)FindComponent("RCGPlayerBody");
	m_Bottom = (CColliderBox2D*)FindComponent("RCGPlayerBottom");
	m_Shadow = (CSpriteComponent*)FindComponent("RCGPlayerShadow");
}

CRCGPlayer::~CRCGPlayer()
{
	const std::vector<CColliderComponent*> vecTileColliders = m_Scene->GetTileColliders();

	size_t size = vecTileColliders.size();

	if (size <= 0)
		return;

	for (size_t i = 0; i < size; i++)
	{
		CColliderBox2D* box = (CColliderBox2D*)vecTileColliders[i];
		box->DeleteCollisionCallback((CGameObject*)this);
	}
}


void CRCGPlayer::Start()
{
	CGameObject::Start();
	const std::vector<CColliderComponent*> vecTileColliders = m_Scene->GetTileColliders();

	size_t size = vecTileColliders.size();

	if (size <= 0)
		return;

	for (size_t i = 0; i < size; i++)
	{
		CColliderBox2D* box = (CColliderBox2D*)vecTileColliders[i];

		box->AddCollisionCallback<CRCGPlayer>(Collision_State::Begin, this, &CRCGPlayer::OnWallCollision);
		box->AddCollisionCallback<CRCGPlayer>(Collision_State::End, this, &CRCGPlayer::OnWallCollisionExit);
	}
}

bool CRCGPlayer::Init()
{
	m_Transform = CreateComponent<CSceneComponent>("RCGPlayerTransform");

	m_Transform->SetPivot(0.5f, 0.0f, 0.0f);

	m_Sprite = CreateComponent<CSpriteComponent>("RCGPlayerSprite");

	m_Sprite->SetLayerName("Default");

	m_Sprite->SetTransparency(true);

	m_Sprite->SetPivot(0.5f, 0.0f, 0.0);

	m_Shadow = CreateComponent<CSpriteComponent>("RCGPlayerShadow");

	m_Shadow->SetLayerName("Shadow");

	m_Shadow->SetTransparency(true);

	CResourceManager::GetInst()->LoadTexture("Shadow", TEXT("shadow.png"));

	m_Shadow->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "PlayerShadow",
	CResourceManager::GetInst()->FindTexture("Shadow"));

	m_Shadow->SetPivot(0.5f, 0.5f, 0.0f);

	m_Shadow->SetRelativeScale(100.0f, 25.0f, 1.0f);

	m_Body = CreateComponent<CColliderBox2D>("RCGPlayerBody");

	m_Body->SetCollisionProfile("Player");

	m_Body->SetOffset(0.0f, 80.0f, 0.0f);

	m_Body->SetExtent(60.0f, 60.0f);

	m_ContactBox = CreateComponent<CColliderBox2D>("RCGPlayerContactBox");

	m_ContactBox->SetCollisionProfile("PlayerContactBox");

	m_ContactBox->SetExtent(40.0f, 60.0f);

	m_ContactBox->SetOffset(0.0f, 80.0f, 0.0f);

	// m_ContactBox->AddCollisionCallback(Collision_State::Begin, this, &CRCGPlayer::OnEnemyContact);


	m_Bottom = CreateComponent<CColliderBox2D>("RCGPlayerBottom");

	m_Bottom->SetCollisionProfile("PlayerBottom");

	m_Bottom->SetExtent(60.0f, 10.0f);

	m_HitBox = CreateComponent<CColliderBox2D>("RCGPlayerAttack");

	m_HitBox->SetCollisionProfile("PlayerAttack");

	m_HitBox->Enable(false);

	m_NavAgent = CreateComponent<CNavAgent>("NavAgent");

	m_Transform->AddChild(m_Sprite);

	m_Transform->AddChild(m_Body);

	m_Transform->AddChild(m_Bottom);

	m_Bottom->AddChild(m_Shadow);

	m_Transform->AddChild(m_HitBox);

	m_Body->AddChild(m_ContactBox);

	m_Bottom->SetRelativePos(0.0f, -70.0f, 0.0f);

	m_WalkSpeed = 300.0f;

	m_RunSpeed = 600.0f;

	m_CurrentTime = 0.0f;

	m_CurrentAttackInputFlag = 0;

	m_DoubleTapCheckTime = 0.2f;

	m_JumpSpeed = 70.0f;

	m_Gravity = -16.0f;

	m_IsRunning = false;

	m_RunTap = false;

	m_AbleToMove = true;

	m_Dodge = false;

	m_OnGround = false;

	m_Jump = false;

	m_WallJump = false;

	m_Guard = false;

	m_AbleToAttack = true;

	m_GravityEnable = true;

	m_Physics = false;

	m_BattleStart = false;

	m_JumpTime = 0.0f;

	m_CurrentState = (int)EKyokoState::NORMAL;

	m_MaxHealth = 160;

	m_Sprite->SetWorldPos(m_Transform->GetWorldPos().x, -75.0f, m_Transform->GetWorldPos().z);
	m_Body->SetWorldPos(m_Transform->GetWorldPos().x, -75.0f, m_Transform->GetWorldPos().z);

	m_Health = CClientManager::GetInst()->GetManager()->GetHealth();
	m_SpecialGauge = CClientManager::GetInst()->GetManager()->GetSpecialGuage();

	// Locomotion
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveUp", KeyState_Push, this, &CRCGPlayer::MoveUp);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveDown", KeyState_Push, this, &CRCGPlayer::MoveDown);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveLeft", KeyState_Push, this, &CRCGPlayer::MoveLeft);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveRight", KeyState_Push, this, &CRCGPlayer::MoveRight);

	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveUp", KeyState_Up, this, &CRCGPlayer::MoveUpEnd);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveDown", KeyState_Up, this, &CRCGPlayer::MoveDownEnd);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveLeft", KeyState_Up, this, &CRCGPlayer::MoveLeftEnd);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveRight", KeyState_Up, this, &CRCGPlayer::MoveRightEnd);

	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveLeft", KeyState_Down, this, &CRCGPlayer::Run);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveRight", KeyState_Down, this, &CRCGPlayer::Run);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveUp", KeyState_Down, this, &CRCGPlayer::DodgeUp);
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveDown", KeyState_Down, this, &CRCGPlayer::DodgeDown);
	CInput::GetInst()->SetCallback<CRCGPlayer>("Jump", KeyState_Down, this, &CRCGPlayer::JumpStart);
	CInput::GetInst()->SetCallback<CRCGPlayer>("Jump", KeyState_Up, this, &CRCGPlayer::JumpEnd);

	// Nav test
	CInput::GetInst()->SetCallback<CRCGPlayer>("MoveTo", KeyState_Down, this, &CRCGPlayer::MouseRDown);

	return true;
}

void CRCGPlayer::Update(float deltaTime)
{
	SetCustomY(m_Bottom->GetWorldPos().y);

	CGameObject::Update(deltaTime);

	int currentFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

	m_Sprite->SetRelativeScale(m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size.x * 2.0f,
		m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size.y * 2.0f, 1.0f);

	if (m_BattleStart)
	{
		if(!m_OnGround)
			ApplyAbsForce(40.0f * deltaTime, 0.0f);
		return;
	}


	if (!m_IsMovingDown && !m_IsMovingUp && !m_IsMovingLeft && !m_IsMovingRight)
	{
		if (m_IsRunning)
			m_IsRunning = false;
		if (!m_Dodge && m_OnGround && !m_Guard &&
			!CheckState(EKyokoState::ATTACK) &&
			!CheckState(EKyokoState::GETHIT) &&
			!CheckState(EKyokoState::GRABBED) &&
			!CheckState(EKyokoState::GRABBING))
			m_Sprite->ChangeAnimation("Idle");

		m_CurrentMoveFlag |= (int)EMoveDirection::NONE;
	}

	m_CurrentTime += deltaTime;

	int finishedState = PopStateEnd(m_CurrentTime);

	if (m_ContactBox->GetPrevColliderList().empty() && !m_ContactEnemies.empty())
		m_ContactEnemies.clear();

	if (m_Bottom->GetPrevColliderList().empty())
	{
		if (m_TopWallCollision)
			m_TopWallCollision = false;

		if (m_BottomWallCollision)
			m_BottomWallCollision = false;
	}

	if (m_ContactBox->GetPrevColliderList().empty())
	{
		if (m_LeftWallCollision)
			m_LeftWallCollision = false;

		if (m_RightWallCollision)
			m_RightWallCollision = false;
	}


	if (m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Knockdown_fall") && m_OnGround)
	{
		m_Sprite->ChangeAnimation("Knockdown_ground");
		PopStateEnd(EKyokoState::GETHIT);
		m_HitCount = 0;
		m_Physics = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
		PushState(EKyokoState::DOWN, m_CurrentTime + 3.0f);
	}

	if (finishedState & (int)EKyokoState::DOWN)
	{	
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
		if (m_Health > 0)
			m_Sprite->ChangeAnimation("Getup");
		else
			m_Sprite->ChangeAnimation("Defeated");
	}

	if (finishedState & (int)EKyokoState::KNOCKDOWN)
	{
		if (!m_OnGround)
			m_Sprite->ChangeAnimation("Knockdown_midair");
		else
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			m_Physics = false;
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
			PushState(EKyokoState::DOWN, m_CurrentTime + 3.0f);
		}
	}

	if (finishedState & (int)EKyokoState::BLOWNBACK)
	{
		if (!m_OnGround)
			m_Sprite->ChangeAnimation("Blownback_midair");
		else
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			m_Physics = false;
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
			PushState(EKyokoState::DOWN, m_CurrentTime + 3.0f);
		}
	}

	if (finishedState & (int)EKyokoState::STUNNED)
	{
		m_Sprite->ChangeAnimation("Idle");
		PopStateEnd(EKyokoState::GETHIT);
		m_AbleToMove = true;
		m_Stunned = false;
	}

	if (finishedState & (int)EKyokoState::INVINCIBLE)
	{
		m_Sprite->SetOpacity(1.0f);
	}

	if (m_CurrentTime - m_RunTapLastTime > m_DoubleTapCheckTime)
		m_RunTap = false;

	if (m_CurrentTime - m_DodgeTapLastTime > m_DoubleTapCheckTime)
		m_DodgeTap = false;

	if (m_ContactBox->GetPrevColliderList().empty() && !m_ContactEnemies.empty())
		m_ContactEnemies.clear();

	if (m_Dodge)
	{
		switch (m_DodgeDirection)
		{
		case EMoveDirection::UP:
			if (!m_TopWallCollision)
				ApplyForce(0.0f, 70.0f * deltaTime);
			else
				m_Velocity.y = 0.0f;
			break;
		case EMoveDirection::DOWN:
			if (!m_BottomWallCollision)
				ApplyForce(0.0f, -70.0f * deltaTime);
			else
				m_Velocity.y = 0.0f;
			break;
		}
	}
}

void CRCGPlayer::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CRCGPlayer* CRCGPlayer::Clone()
{
	return new CRCGPlayer(*this);
}

void CRCGPlayer::ApplyForce(const Vector2& force, bool isRelative)
{
	m_Acceleration = force;

	m_Velocity += m_Acceleration;

	if (!isRelative)
	{
		m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_Velocity.x);
		m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_Velocity.y);
	}
	else
	{
		m_Sprite->AddWorldPos(GetWorldAxis(AXIS_X) * m_Velocity.x);
		m_Sprite->AddWorldPos(GetWorldAxis(AXIS_Y) * m_Velocity.y);
		m_Body->AddWorldPos(GetWorldAxis(AXIS_X) * m_Velocity.x);
		m_Body->AddWorldPos(GetWorldAxis(AXIS_Y) * m_Velocity.y);
	}
}

void CRCGPlayer::ApplyForce(float x, float y, bool isRelative)
{
	m_Acceleration = Vector2(x, y);

	m_Velocity += m_Acceleration;

	if (!isRelative)
	{
		m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_Velocity.x);
		m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_Velocity.y);
	}
	else
	{
		m_Sprite->AddWorldPos(GetWorldAxis(AXIS_X) * m_Velocity.x);
		m_Sprite->AddWorldPos(GetWorldAxis(AXIS_Y) * m_Velocity.y);
		m_Body->AddWorldPos(GetWorldAxis(AXIS_X) * m_Velocity.x);
		m_Body->AddWorldPos(GetWorldAxis(AXIS_Y) * m_Velocity.y);
	}
}

void CRCGPlayer::ApplyAbsForce(const Vector2& force)
{
	m_AbsAccel = force;

	m_AbsVel += m_AbsAccel;

	m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_AbsVel.x);
	m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_AbsVel.y);
}

void CRCGPlayer::ApplyAbsForce(float x, float y)
{
	m_AbsAccel = Vector2(x, y);

	m_AbsVel += m_AbsAccel;

	m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_AbsVel.x);
	m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_AbsVel.y);
}

int CRCGPlayer::PopStateEnd(const float currentTime)
{
	int finishedState = 0;

	if (m_CurrentState != (int)EKyokoState::NORMAL
		&& m_CurrentState != (int)EKyokoState::ATTACK)
	{
		int state = 1;

		for (int i = 0; i < (int)EKyokoState::MAX; i++)
		{
			if (m_CurrentState & state)
			{
				if (m_StateEnd[i] < currentTime && m_StateEnd[i] != 0.0f)
				{
					finishedState |= state;
					m_CurrentState &= ~state;
				}
			}

			state *= 2;
		}
	}

	return finishedState;
}

void CRCGPlayer::PopStateEnd(EKyokoState state)
{
	m_CurrentState &= ~(int)state;
	m_StateEnd[(int)log2((int)state)] = 0.0f;
}

void CRCGPlayer::PushState(EKyokoState state, float duration)
{
	m_CurrentState |= (int)state;
	m_StateEnd[(int)log2((int)state)] = duration;
}

void CRCGPlayer::PushState(EKyokoState state)
{
	m_CurrentState |= (int)state;
}

void CRCGPlayer::OnGround(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->GetRootComponent() == GetRootComponent() &&
		m_Velocity.y <= 0.0f)
	{
		m_OnGround = true;
		m_Jump = false;

		if (CheckState(EKyokoState::ATTACK))
		{
			PopStateEnd(EKyokoState::ATTACK);
		}

		m_CurrentAttackInputFlag = 0;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
		//CSmokeParticle* smokeParticle = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticle");
		//Vector3 pos = Vector3(m_Transform->GetWorldPos().x - m_Sprite->GetWorldScale().x / 2.0f,
		//    m_Transform->GetWorldPos().y - m_Sprite->GetWorldScale().y / 2.0f, 0.0f);
		//smokeParticle->SetRelativePos(pos);
		//smokeParticle->SetLifeSpan(0.5f);
	}
}

void CRCGPlayer::OnWallCollision(const sCollisionResult& result)
{
	if (result.src->GetGameObject()->GetRootComponent()->TypeCheck<CColliderBox2D>()
		&& result.dest->GetGameObject() == this)
	{
		CColliderComponent* collider = (CColliderComponent*)result.src->GetGameObject()->GetRootComponent();

		sCollisionProfile* profile = collider->GetCollisionProfile();

		if (profile->name == "SideWall")
		{
			if (result.src->GetWorldPos().x < result.dest->GetWorldPos().x)
			{
				if (m_Physics && CheckState(EKyokoState::GETHIT) && fabs(m_KnockbackForce.x) >= 5.0f)
				{
					m_FacingRight = true;
					m_Sprite->ChangeAnimation("Wallhit");
				}
				else
				{
					m_AbsVel.x = 0.0f;
					m_Velocity.x = 0.0f;
					m_LeftWallCollision = true;
				}
			}
			else
			{
				if (m_Physics && CheckState(EKyokoState::GETHIT) && fabs(m_KnockbackForce.x) >= 5.0f)
				{
					m_FacingRight = false;
					m_Sprite->ChangeAnimation("Wallhit");
				}
				else
				{
					m_AbsVel.x = 0.0f;
					m_Velocity.x = 0.0f;
					m_RightWallCollision = true;
				}
			}
		}
		else
		{
			if (result.src->GetWorldPos().y < result.dest->GetWorldPos().y)
				m_BottomWallCollision = true;
			else
				m_TopWallCollision = true;
		}
	}
	else if (result.src->TypeCheck<CColliderBox2D>() && result.dest->GetGameObject() == this)
	{
		CColliderComponent* collider = (CColliderComponent*)result.src;

		sCollisionProfile* profile = collider->GetCollisionProfile();

		if (profile->name == "SideWall")
		{
			if (result.src->GetWorldPos().x < result.dest->GetWorldPos().x)
			{
				if (m_Physics && CheckState(EKyokoState::GETHIT) && fabs(m_KnockbackForce.x) >= 5.0f)
				{
					m_FacingRight = true;
					m_Sprite->ChangeAnimation("Wallhit");
				}
				else
				{
					m_AbsVel.x = 0.0f;
					m_Velocity.x = 0.0f;
					m_LeftWallCollision = true;
				}
			}
			else
			{
				if (m_Physics && CheckState(EKyokoState::GETHIT) && fabs(m_KnockbackForce.x) >= 5.0f)
				{
					m_FacingRight = false;
					m_Sprite->ChangeAnimation("Wallhit");
				}
				else
				{
					m_AbsVel.x = 0.0f;
					m_Velocity.x = 0.0f;
					m_RightWallCollision = true;
				}
			}
		}
		else
		{
			CColliderBox2D* pilarBottom = (CColliderBox2D*)result.src;
			sBox2DInfo pilarBottomInfo = pilarBottom->GetInfo();
			sBox2DInfo playerBottomInfo = m_Bottom->GetInfo();

			if ((pilarBottomInfo.center.y + pilarBottomInfo.length.y) < playerBottomInfo.center.y)
				m_BottomWallCollision = true;
			else if(playerBottomInfo.center.y < (pilarBottomInfo.center.y - pilarBottomInfo.length.y))
				m_TopWallCollision = true;
		}
	}
}

void CRCGPlayer::OnWallCollisionExit(const sCollisionResult& result)
{
	if (result.src->GetGameObject()->GetRootComponent()->TypeCheck<CColliderBox2D>()
		&& result.dest->GetGameObject() == this)
	{
		CColliderComponent* collider = (CColliderComponent*)result.src->GetGameObject()->GetRootComponent();

		sCollisionProfile* profile = collider->GetCollisionProfile();

		if (profile->name == "SideWall")
		{
			if (result.src->GetWorldPos().x < result.dest->GetWorldPos().x)
			{
				if (m_IsMovingRight)
					m_LeftWallCollision = false;
			}
			else
			{
				if (m_IsMovingLeft)
					m_RightWallCollision = false;
			}
		}
		else
		{
			if (result.src->GetWorldPos().y < result.dest->GetWorldPos().y)
			{
				if (m_IsMovingUp)
					m_BottomWallCollision = false;
			}
			else
			{
				if (m_IsMovingDown)
					m_TopWallCollision = false;
			}
		}
	}
	else if (result.src->TypeCheck<CColliderBox2D>() && result.dest->GetGameObject() == this)
	{
		CColliderComponent* collider = (CColliderComponent*)result.src;

		sCollisionProfile* profile = collider->GetCollisionProfile();

		if (profile->name == "SideWall")
		{
			if (result.src->GetWorldPos().x < result.dest->GetWorldPos().x)
			{
				if (m_IsMovingRight)
					m_LeftWallCollision = false;
			}
			else
			{
				if (m_IsMovingLeft)
					m_RightWallCollision = false;
			}
		}
		else
		{
			CColliderBox2D* pilarBottom = (CColliderBox2D*)result.src;
			sBox2DInfo pilarBottomInfo = pilarBottom->GetInfo();
			sBox2DInfo playerBottomInfo = m_Bottom->GetInfo();

			if ((pilarBottomInfo.center.y + pilarBottomInfo.length.y) < playerBottomInfo.center.y)
			{
				m_BottomWallCollision = false;
			}
			else if (playerBottomInfo.center.y < (pilarBottomInfo.center.y - pilarBottomInfo.length.y))
			{
				m_TopWallCollision = false;
			}
		}
	}
}

void CRCGPlayer::MoveUp(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::STUNNED))
		return;

	if (m_Guard)
		return;

	m_CurrentMoveFlag &= ~(int)EMoveDirection::NONE;
	m_CurrentMoveFlag |= (int)EMoveDirection::UP;

	if (CheckState(EKyokoState::GRABBING))
		return;

	if (!m_AbleToMove)
		return;

	if (!m_IsMovingUp)
		m_IsMovingUp = true;


	if (m_OnGround && CheckState(EKyokoState::ATTACK))
		return;

	if (!m_IsRunning)
	{
		if (!m_TopWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_WalkSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Walk");
		}
	}
	else
	{
		if (!m_TopWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_WalkSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Run");
		}
	}
}

void CRCGPlayer::MoveUpEnd(float deltaTime)
{
	m_IsMovingUp = false;
	m_CurrentMoveFlag &= ~(int)EMoveDirection::UP;
}

void CRCGPlayer::MoveDown(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::STUNNED))
		return;

	if (m_Guard)
		return;


	m_CurrentMoveFlag &= ~(int)EMoveDirection::NONE;
	m_CurrentMoveFlag |= (int)EMoveDirection::DOWN;

	if (CheckState(EKyokoState::GRABBING))
		return;

	if (!m_AbleToMove)
		return;

	if (!m_IsMovingDown)
		m_IsMovingDown = true;

	if (m_OnGround && CheckState(EKyokoState::ATTACK))
		return;

	if (!m_IsRunning)
	{
		if (!m_BottomWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * -m_WalkSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Walk");
		}
	}
	else
	{
		if (!m_BottomWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * -m_RunSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Run");
		}
	}
}

void CRCGPlayer::MoveDownEnd(float deltaTime)
{
	m_IsMovingDown = false;
	m_CurrentMoveFlag &= ~(int)EMoveDirection::DOWN;
}

void CRCGPlayer::MoveLeft(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::STUNNED))
		return;

	if (m_Guard)
		return;


	m_CurrentMoveFlag &= ~(int)EMoveDirection::NONE;
	m_CurrentMoveFlag |= (int)EMoveDirection::LEFT;

	if (CheckState(EKyokoState::GRABBING))
		return;

	if (!m_AbleToMove)
		return;

	if (!m_IsMovingLeft)
		m_IsMovingLeft = true;

	if (m_OnGround && CheckState(EKyokoState::ATTACK))
		return;

	if (m_FacingRight)
	{
		m_FacingRight = false;
		m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);
	}

	if (!m_IsRunning)
	{
		if (!m_LeftWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * -m_WalkSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Walk");
		}
	}
	else
	{
		if (!m_LeftWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * -m_RunSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Run");
		}
	}
}

void CRCGPlayer::MoveLeftEnd(float deltaTime)
{
	m_IsMovingLeft = false;
	m_CurrentMoveFlag &= ~(int)EMoveDirection::LEFT;
}

void CRCGPlayer::MoveRight(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::STUNNED))
		return;

	if (m_Guard)
		return;

	m_CurrentMoveFlag &= ~(int)EMoveDirection::NONE;
	m_CurrentMoveFlag |= (int)EMoveDirection::RIGHT;

	if (CheckState(EKyokoState::GRABBING))
		return;

	if (!m_AbleToMove)
		return;

	if (!m_IsMovingRight)
		m_IsMovingRight = true;

	if (m_OnGround && CheckState(EKyokoState::ATTACK))
		return;

	if (!m_FacingRight)
	{
		m_FacingRight = true;
		m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
	}


	if (!m_IsRunning)
	{
		if (!m_RightWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_WalkSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Walk");
		}

	}
	else
	{
		if (!m_RightWallCollision)
			m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_RunSpeed * deltaTime);

		if (m_OnGround)
		{
			if (!m_Dodge)
				m_Sprite->ChangeAnimation("Run");
		}
	}
}

void CRCGPlayer::MoveRightEnd(float deltaTime)
{
	m_IsMovingRight = false;
	m_CurrentMoveFlag &= ~(int)EMoveDirection::RIGHT;
}

void CRCGPlayer::JumpStart(float deltaTime)
{
	if (m_CurrentState != (int)EKyokoState::NORMAL)
		return;
	 
	if (m_Dodge || m_Guard || !m_AbleToMove)
		return;

	if (m_OnGround && !m_Jump)
	{
		m_Jump = true;
		m_JumpTime = m_CurrentTime;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_OnGround = false;
		CResourceManager::GetInst()->SoundPlay("Jump");
		m_Sprite->ChangeAnimation("Jump");

		//CSmokeParticle* smokeParticle = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticle");
		//Vector3 pos = Vector3(m_Transform->GetWorldPos().x - m_Sprite->GetWorldScale().x / 2.0f,
		//    m_Transform->GetWorldPos().y - m_Sprite->GetWorldScale().y / 2.0f, 0.0f);
		//smokeParticle->SetRelativePos(pos);
		//smokeParticle->SetLifeSpan(0.5f);
	}
	else if (!m_OnGround && !m_WallJump)
	{
		if (m_LeftWallCollision)
		{
			m_WallJump = true; 
			m_WallJumpSpeed = 25.0f;
			if (!m_FacingRight)
			{
				m_FacingRight = true;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
			}
			m_AbleToMove = false;
			CResourceManager::GetInst()->SoundPlay("Grab_wall");
			m_Sprite->ChangeAnimation("Walljump");
		}
		else if (m_RightWallCollision)
		{
			m_WallJump = true;
			m_WallJumpSpeed = -25.0f;
			if (m_FacingRight)
			{
				m_FacingRight = false;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);
			}
			m_AbleToMove = false;
			CResourceManager::GetInst()->SoundPlay("Grab_wall");
			m_Sprite->ChangeAnimation("Walljump");
		}
	}
}

void CRCGPlayer::JumpEnd(float deltaTime)
{
	if (m_Jump)
	{
		m_Jump = false;

		if (m_Velocity.y >= 0.0f && m_Velocity.y <= 2.0f)
			m_Velocity.y = 1.5f;
	}
}

void CRCGPlayer::Run(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::GRABBING))
		return;

	if (!m_OnGround || m_Guard || !m_AbleToMove)
		return;

	if (m_RunTap)
	{
		m_RunTap = false;

		if (m_CurrentTime - m_RunTapLastTime < m_DoubleTapCheckTime)
		{
			m_IsRunning = true;

			/*CSmokeParticle* smokeParticle = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticle");
			Vector3 pos = Vector3(m_Transform->GetWorldPos().x - m_Sprite->GetWorldScale().x / 2.0f,
				m_Transform->GetWorldPos().y - m_Sprite->GetWorldScale().y / 2.0f, 0.0f);
			smokeParticle->SetRelativePos(pos);
			smokeParticle->SetLifeSpan(0.5f);*/
		}
	}
	else
	{
		m_RunTapLastTime = m_CurrentTime;
		m_RunTap = true;
	}
}

void CRCGPlayer::DodgeUp(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::GRABBING))
		return;

	if (m_Dodge || m_Jump || m_Guard || !m_OnGround || !m_AbleToMove)
		return;

	if (m_DodgeTap)
	{
		m_DodgeTap = false;

		if (m_CurrentTime - m_DodgeTapLastTime < m_DoubleTapCheckTime)
		{
			m_Sprite->ChangeAnimation("Dodge");
			m_Velocity = Vector2(0.0f, 0.0f);
			m_DodgeDirection = EMoveDirection::UP;
			m_Dodge = true;
		}
	}
	else
	{
		m_DodgeTapLastTime = m_CurrentTime;
		m_DodgeTap = true;
	}
}

void CRCGPlayer::DodgeDown(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) || CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED) || CheckState(EKyokoState::GRABBING))
		return;

	if (m_Dodge || m_Jump || m_Guard || !m_OnGround || !m_AbleToMove)
		return;

	if (m_DodgeTap)
	{
		m_DodgeTap = false;

		if (m_CurrentTime - m_DodgeTapLastTime < m_DoubleTapCheckTime)
		{
			m_Sprite->ChangeAnimation("Dodge");
			m_Velocity = Vector2(0.0f, 0.0f);
			m_DodgeDirection = EMoveDirection::DOWN;
			m_Dodge = true;
		}
	}
	else
	{
		m_DodgeTapLastTime = m_CurrentTime;
		m_DodgeTap = true;
	}
}

void CRCGPlayer::DodgeEnd()
{
	m_Dodge = false;
	m_Velocity = Vector2(0.0f, 0.0f);
}

void CRCGPlayer::Guard(float deltaTime)
{
	if (m_Dodge || !m_OnGround || m_Jump || !m_AbleToMove)
		return;

	m_Guard = true;

	m_IsRunning = false;
	m_IsMovingUp = false;
	m_IsMovingDown = false;
	m_IsMovingLeft = false;
	m_IsMovingRight = false;
	m_AbleToMove = false;


	m_Sprite->ChangeAnimation("Block");
}

void CRCGPlayer::GuardEnd(float deltaTime)
{
	m_Guard = false;
}

void CRCGPlayer::WallJump()
{
	m_WallJumpTime = m_CurrentTime;
	m_Jump = true;
	m_JumpTime = m_CurrentTime - 0.05f;;
	m_JumpSpeed = 20.0f;
	m_Sprite->ChangeAnimation("Jump");
	m_LeftWallCollision = false;
	m_RightWallCollision = false;
	m_BottomWallCollision = false;
	m_TopWallCollision = false;
	m_Invincible = false;
	m_GravityEnable = true;
	m_AbleToAttack = true;
	m_AbleToMove = true;
}

void CRCGPlayer::GutPunch()
{
	PushState(EKyokoState::ATTACK);
	int randomSoundIndex = (rand() % 3) + 1;

	std::string name = "Punch" + std::to_string(randomSoundIndex);

	CResourceManager::GetInst()->SoundPlay(name);

	((CRCGEnemy*)m_Enemy)->GetHit(EAttackType::HIT, Vector2(0.0f, 0.0f), 4, 0.0f, 0.0f, false);
}

void CRCGPlayer::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
	m_HitBox->Enable(false);

	if (!m_GravityEnable)
		m_GravityEnable = true;

	if (m_OnGround && CheckState(EKyokoState::STUNNED))
	{
		PopStateEnd(EKyokoState::STUNNED);
		m_Stunned = false;
	}

	if (m_OnGround)
	{
		if (type == EAttackType::HIT)
		{
			switch (m_HitCount)
			{
			case 0:
				m_Sprite->ChangeAnimation("Hit1");
				break;
			case 1:
				m_Sprite->ChangeAnimation("Hit2");
				break;
			case 2:
				m_Sprite->ChangeAnimation("Hit3");
				break;
			}

			m_HitCount++;
			m_HitCount %= m_HitAnimCnt;
		}
		else if (type == EAttackType::KNOCKDOWN)
		{
			m_KnockbackForce = dir * force;

			if (CheckState(EKyokoState::STUNNED))
			{
				PopStateEnd(EKyokoState::STUNNED);
				m_Stunned = false;
			}

			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
			}
			else
			{
				m_FacingRight = false;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);
			}

			m_Sprite->ChangeAnimation("Knockdown");

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_OnGround && m_KnockbackForce.y > 0.0f)
			{
				m_Velocity.y = 0.001f;
				m_OnGround = false;
			}

			m_Physics = true;


			PushState(EKyokoState::KNOCKDOWN, m_CurrentTime + forceTime);
		}
		else
		{
			m_KnockbackForce = dir * force;

			if (CheckState(EKyokoState::STUNNED))
			{
				PopStateEnd(EKyokoState::STUNNED);
				m_Stunned = false;
			}
			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
			}
			else
			{
				m_FacingRight = false;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);
			}

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_OnGround && m_KnockbackForce.y > 0.0f)
			{
				m_Velocity.y = 0.001f;
				m_OnGround = false;
			}

			m_Sprite->ChangeAnimation("Blownback");

			m_Physics = true;


			PushState(EKyokoState::BLOWNBACK, m_CurrentTime + forceTime);
		}
	}
	else
	{
		if (type == EAttackType::KNOCKDOWN || type == EAttackType::HIT)
		{
			m_KnockbackForce = dir * force;

			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
			}
			else
			{
				m_FacingRight = false;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);
			}

			m_Sprite->ChangeAnimation("Knockdown");

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_OnGround && m_KnockbackForce.y > 0.0f)
			{
				m_Velocity.y = 0.001f;
				m_OnGround = false;
			}

			m_Physics = true;

			PushState(EKyokoState::KNOCKDOWN, m_CurrentTime + forceTime);
		}
		else
		{
			m_KnockbackForce = dir * force;

			if (CheckState(EKyokoState::STUNNED))
			{
				PopStateEnd(EKyokoState::STUNNED);
				m_Stunned = false;
			}
			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
			}
			else
			{
				m_FacingRight = false;
				m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);
			}

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_OnGround && m_KnockbackForce.y > 0.0f)
			{
				m_Velocity.y = 0.001f;
				m_OnGround = false;
			}

			m_Sprite->ChangeAnimation("Blownback");

			m_Physics = true;


			PushState(EKyokoState::BLOWNBACK, m_CurrentTime + forceTime);
		}
	}

	m_Health -= damage;
	if (m_Health > 0 && m_Health != m_MaxHealth)
	{
		m_MainWidget->SetEmpty();
		int iterations = m_Health / 10;
		int remain = m_Health % 10;
		int i;
		for (i = 0; i < iterations; i++)
		{
			m_MainWidget->FullHPBar(i);
		}

		m_MainWidget->SetHPOpcity(i, remain / (float)10.0f);

	}

	if (m_Health <= 0)
	{
		m_AbleToAttack = false;
		m_AbleToMove = false;
	}

	PopStateEnd(EKyokoState::ATTACK);
	PushState(EKyokoState::GETHIT);
}

void CRCGPlayer::HitStatusEnd()
{
	if (m_OnGround && !m_Stunned)
	{
		m_Sprite->ChangeAnimation("Idle");
		PopStateEnd(EKyokoState::GETHIT);
	}
	else if (m_OnGround && m_Stunned)
	{
		m_Sprite->ChangeAnimation("Stunned");
		CResourceManager::GetInst()->SoundPlay("Dizzy");
		m_Stunned = false;
		PushState(EKyokoState::STUNNED, m_CurrentTime + 3.0f);
	}
	else
		m_Sprite->ChangeAnimation("Knockdown");

	m_AbleToAttack = true;

}

void CRCGPlayer::AddWallCollisionCallback(CColliderBox2D* collider)
{
	collider->AddCollisionCallback<CRCGPlayer>(Collision_State::Begin, this, &CRCGPlayer::OnWallCollision);
	collider->AddCollisionCallback<CRCGPlayer>(Collision_State::End, this, &CRCGPlayer::OnWallCollisionExit);
}

void CRCGPlayer::DeleteWallCollisionCallback(CColliderBox2D* collider)
{
	collider->DeleteCollisionCallback((CGameObject*)this);
}

void CRCGPlayer::OnEnemyContact(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
	{
		CColliderComponent* comp = result.dest;

		auto iter = std::find(m_ContactEnemies.begin(), m_ContactEnemies.end(), comp);

		if (iter != m_ContactEnemies.end())
			return;

		m_ContactEnemies.push_back(comp);
	}
}

void CRCGPlayer::OnEnemyContactExit(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
	{
		CColliderComponent* comp = result.dest;

		auto iter = std::find(m_ContactEnemies.begin(), m_ContactEnemies.end(), comp);

		if (iter != m_ContactEnemies.end())
			return;

		m_ContactEnemies.erase(iter);
	}
}

void CRCGPlayer::SetHP()
{
	m_MainWidget->SetEmpty();
	if (m_Health > 0)
	{
		int iterations = m_Health / 10;
		int remain = m_Health % 10;
		int i;
		for (i = 0; i < iterations; i++)
		{
			m_MainWidget->FullHPBar(i);
		}

		if(i != (m_MaxHealth / 10))
			m_MainWidget->SetHPOpcity(i, remain / (float)10.0f);
	}
}

void CRCGPlayer::SetSP()
{
	m_MainWidget->SetSpecialPercent(m_SpecialGauge / 100.0f);
}

void CRCGPlayer::Defeated()
{
	CResourceManager::GetInst()->SoundPlay("Dizzy");
}

void CRCGPlayer::GameOver()
{
	CResourceManager::GetInst()->SoundPlay("GameOver");
	CEngine::GetInst()->SetPlay(false);
}

void CRCGPlayer::MouseRDown(float deltaTime)
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	CTileMapComponent* tileMap = m_Scene->GetNavigationManager()->GetNavData();
	Vector2 offset = Vector2(tileMap->GetTileOffsetX(), tileMap->GetTileOffsetY());
	Move(Vector3(mousePos.x - offset.x, mousePos.y - offset.y, 0.0f));
}