#include "RCGEnemy.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Engine.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Scene/SceneManager.h"
#include "RCGPlayer.h"
#include "Component/ColliderComponent.h"
#include "../Widget/RCGEnemyHUD.h"
#include "LockEventTrigger.h"
#include "../ClientManager.h"
#include "../RCGGameManager.h"
#include "ChainLockWall.h"

CRCGEnemy::CRCGEnemy() :
	m_Transform(nullptr),
	m_Body(nullptr),
	m_Bottom(nullptr),
	m_Sprite(nullptr),
	m_StateEnd{},
	m_HitCount(0),
	m_CurrentTime(0.0f)
{
	SetTypeID<CRCGEnemy>();
	m_Opacity = 1.0f;
	m_FacingRight = true;
}

CRCGEnemy::CRCGEnemy(const CRCGEnemy& obj)
{
	m_Transform = (CSceneComponent*)FindComponent("RCGEnemyTransform");
	m_Sprite = (CSpriteComponent*)FindComponent("RCGEnemySprite");
	m_Body = (CColliderBox2D*)FindComponent("RCGEnemyBody");
	m_Bottom = (CColliderBox2D*)FindComponent("RCGEnemyBottom");
	m_HUDWidget = (CWidgetComponent*)FindComponent("EnemyHUD");
	m_Shadow = (CSpriteComponent*)FindComponent("RCGEnemyShadow");
}

CRCGEnemy::~CRCGEnemy()
{
	CRCGPlayer* player = (CRCGPlayer*)m_Scene->GetPlayerObject();

	player->DeleteContactEnemy(m_ContactBox);

	const std::vector<CColliderComponent*> vecTileColliders = m_Scene->GetTileColliders();

	size_t size = vecTileColliders.size();

	if (size <= 0)
		return;

	for (size_t i = 0; i < size; i++)
	{
		CColliderBox2D* box = (CColliderBox2D*)vecTileColliders[i];
		box->DeleteCollisionCallback((CGameObject*)this);
	}

	CClientManager::GetInst()->GetManager()->DeleteEnemy(this);

	CLockEventTrigger* currentEvent = CClientManager::GetInst()->GetManager()->GetCurrentEvent();

	if (currentEvent)
	{
		const std::vector<CChainLockWall*> vecChainWalls = currentEvent->GetChainWalls();

		size_t size = vecChainWalls.size();

		for (size_t i = 0; i < size; i++)
		{
			if (vecChainWalls[i]->GetRootComponent()->TypeCheck<CColliderBox2D>())
			{
				CColliderBox2D* box = (CColliderBox2D*)vecChainWalls[i]->GetRootComponent();
				box->DeleteCollisionCallback((CGameObject*)this);
			}
		}

		currentEvent->DeleteEnemy(this);
	}
}

void CRCGEnemy::Start()
{
	CGameObject::Start();

	const std::vector<CColliderComponent*> vecTileColliders = m_Scene->GetTileColliders();

	size_t size = vecTileColliders.size();

	if (size <= 0)
		return;

	for (size_t i = 0; i < size; i++)
	{
		CColliderBox2D* box = (CColliderBox2D*)vecTileColliders[i];

		box->AddCollisionCallback<CRCGEnemy>(Collision_State::Begin, this, &CRCGEnemy::OnWallCollision);
		box->AddCollisionCallback<CRCGEnemy>(Collision_State::End, this, &CRCGEnemy::OnWallCollisionExit);
	}
}

bool CRCGEnemy::Init()
{
	m_Transform = CreateComponent<CSceneComponent>("RCGEnemyTransform");

	m_Transform->SetPivot(0.5f, 0.0f, 0.0f);

	m_Sprite = CreateComponent<CSpriteComponent>("RCGEnemySprite");

	m_Sprite->SetTransparency(true);

	m_Sprite->SetPivot(0.5f, 0.0f, 0.0f);

	m_Shadow = CreateComponent<CSpriteComponent>("RCGEnemyShadow");

	m_Shadow->SetLayerName("Shadow");

	m_Shadow->SetTransparency(true);

	CResourceManager::GetInst()->LoadTexture("Shadow", TEXT("shadow.png"));

	m_Shadow->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "EnemyShadow",
		CResourceManager::GetInst()->FindTexture("Shadow"));

	m_Shadow->SetPivot(0.5f, 0.5f, 0.0f);

	m_Shadow->SetRelativeScale(100.0f, 25.0f, 1.0f);

	m_Body = CreateComponent<CColliderBox2D>("RCGEnemyBody");

	m_Body->SetOffset(0.0f, 80.0f, 0.0f);

	m_Body->SetExtent(60.0f, 60.0f);

	m_Body->SetCollisionProfile("Enemy");

	m_ContactBox = CreateComponent<CColliderBox2D>("RCGEnemyContactBox");

	m_ContactBox->SetExtent(40.0f, 60.0f);

	m_ContactBox->SetOffset(0.0f, 80.0f, 0.0f);

	m_ContactBox->SetCollisionProfile("EnemyContactBox");

	m_Bottom = CreateComponent<CColliderBox2D>("RCGEnemyBottom");

	m_Bottom->SetCollisionProfile("EnemyBottom");

	m_Bottom->SetExtent(60.0f, 10.0f);

	m_HitBox = CreateComponent<CColliderBox2D>("RCGEnemyAttack");

	m_HitBox->Enable(false);

	m_HitBox->SetCollisionProfile("EnemyAttack");

	m_Transform->AddChild(m_Sprite);

	m_Transform->AddChild(m_Body);

	m_Transform->AddChild(m_Bottom);

	m_Transform->AddChild(m_HitBox);

	m_Body->AddChild(m_ContactBox);

	m_Bottom->SetRelativePos(0.0f, -70.0f, 0.0f);

	m_Shadow->SetRelativePos(0.0f, -10.0f, 0.0f);

	m_Bottom->AddChild(m_Shadow);

	m_Bottom->AddCollisionCallback<CRCGEnemy>(Collision_State::Begin, this, &CRCGEnemy::OnGround);

	m_NavAgent = CreateComponent<CNavAgent>("NavAgent");

	m_Sprite->SetWorldPos(m_Transform->GetWorldPos().x, -75.0f, m_Transform->GetWorldPos().z);
	m_Body->SetWorldPos(m_Transform->GetWorldPos().x, -75.0f, m_Transform->GetWorldPos().z);

	m_WalkSpeed = 250.0f;

	m_RunSpeed = 550.0f;

	m_JumpSpeed = 40.0f;

	m_OnGround = false;

	m_JumpTime = 0.0f;

	m_Jump = false;

	m_Physics = false;

	m_Gravity = -10.0f;

	m_Invincible = false;

	m_Push = false;

	m_GravityEnable = true;

	m_Spawning = false;

	m_StunAccumulate = 0.0f;

	m_HealthPoint = 350;

	m_MaxFallSpeed = -30.0f;

	m_SpawnTime = 0.0f;

	m_CurrentState = (int)EEnemyState::NORMAL;

	return true;
}

void CRCGEnemy::Update(float deltaTime)
{
	SetCustomY(m_Bottom->GetWorldPos().y);

	CGameObject::Update(deltaTime);

	int currentFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

	m_Sprite->SetRelativeScale(m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size.x * 2.0f,
		m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size.y * 2.0f, 1.0f);

	if (m_Spawning)
	{
		m_Invincible = true;

		m_SpawnTime += deltaTime;

		if (m_SpawnTime > 1.0f)
		{
			m_Invincible = false;
			m_Spawning = false;
			m_SpawnTime = 1.0f;
		}

		m_Sprite->SetOpacity(m_SpawnTime);

		return;
	}

	if (m_FacingRight)
		m_Sprite->GetAnimationInstance()->SetHorizontalFlip(false);
	else
		m_Sprite->GetAnimationInstance()->SetHorizontalFlip(true);

	if (m_ContactBox->GetPrevColliderList().empty())
	{
		m_LeftWallCollision = false;
		m_RightWallCollision = false;
	}

	if (!m_OnGround && m_GravityEnable)
	{
		if (m_Jump)
		{
			if (m_CurrentTime - m_JumpTime <= 0.2f)
			{
				ApplyForce(0.0f, m_JumpSpeed * deltaTime, true);

				if (m_Velocity.y >= 15.0f)
					m_Velocity.y = 15.0f;
			}
			else
			{
				m_Jump = false;
			}
		}
		else if (CheckState(EEnemyState::KNOCKDOWN) || CheckState(EEnemyState::BLOWNBACK))
		{
			ApplyForce(0.0f, m_KnockbackForce.y * deltaTime, true);
			ApplyForce(0.0f, m_KnockbackForce.y * -0.01f * deltaTime, true);

			if (m_Velocity.y >= 10.0f)
				m_Velocity.y = 10.0f;
		}

		if (!CheckState(EEnemyState::KNOCKDOWN) && !CheckState(EEnemyState::BLOWNBACK))
		{
			ApplyForce(0.0f, m_Gravity * deltaTime, true);

			if (m_Velocity.y <= m_MaxFallSpeed)
				m_Velocity.y = m_MaxFallSpeed;

		}

		if (m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Jump"))
		{
			if (m_Velocity.y > 0.0f)
				m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(0);
			else if (m_Velocity.y < 0.0f && m_Velocity.y > -0.5f)
				m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(1);
			else
				m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(2);
		}

	}

	if (CheckState(EEnemyState::KNOCKDOWN) ||
		CheckState(EEnemyState::BLOWNBACK))
	{
		if (m_OnGround)
		{
			if (!m_LeftWallCollision && m_KnockbackForce.x < 0.0f)
			{
				ApplyForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else if (!m_RightWallCollision && m_KnockbackForce.x > 0.0f)
			{
				ApplyForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else
				m_Velocity.x = 0.0f;
		}
		else
		{
			if (!m_LeftWallCollision && m_KnockbackForce.x < 0.0f)
			{

				ApplyAbsForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyAbsForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else if (!m_RightWallCollision && m_KnockbackForce.x > 0.0f)
			{
				ApplyAbsForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyAbsForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else
				m_AbsVel.x = 0.0f;
		}
	}
	else if (m_Physics)
	{
		Vector2 v = m_Velocity;

		v *= -1.0f;

		v *= 0.1f;

		ApplyForce(v * deltaTime, true);

		v = m_AbsVel;

		v *= -1.0f;

		v *= 0.1f;

		if (!m_LeftWallCollision && v.x < 0.0f)
			ApplyAbsForce(v * deltaTime);
		else if (!m_RightWallCollision && v.x > 0.0f)
			ApplyAbsForce(v * deltaTime);
	}


	m_CurrentTime += deltaTime;
}

void CRCGEnemy::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CRCGEnemy* CRCGEnemy::Clone()
{
	return new CRCGEnemy(*this);
}

void CRCGEnemy::ApplyForce(const Vector2& force, bool isRelative)
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

void CRCGEnemy::ApplyForce(float x, float y, bool isRelative)
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

void CRCGEnemy::ApplyAbsForce(const Vector2& force)
{
	m_AbsAccel = force;

	m_AbsVel += m_AbsAccel;

	m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_AbsVel.x);
	m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_AbsVel.y);
}

void CRCGEnemy::ApplyAbsForce(float x, float y)
{
	m_AbsAccel = Vector2(x, y);

	m_AbsVel += m_AbsAccel;

	m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_AbsVel.x);
	m_Transform->AddWorldPos(GetWorldAxis(AXIS_Y) * m_AbsVel.y);
}

void CRCGEnemy::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
	if (m_Invincible)
		return;

	m_Push = false;

	m_HitBox->Enable(false);

	if (CheckState(EEnemyState::STUNNED) &&
		m_OnGround && 
		m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Stunned"))
	{
		PopStateEnd(EEnemyState::STUNNED);
		m_Stunned = false;
	}

	if (CheckState(EEnemyState::PARALYSIS))
		PopStateEnd(EEnemyState::PARALYSIS);

	if (m_OnGround)
	{
		if (type == EAttackType::HIT)
		{
			if (CheckState(EEnemyState::GRABBED))
			{
				m_Sprite->ChangeAnimation("Heldhit");
			}
			else if (!CheckState(EEnemyState::DOWN))
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

				if(!m_Stunned)
					m_StunAccumulate += 0.1f;
			}
			else
				m_Sprite->ChangeAnimation("Groundhit");

			m_HealthPoint -= damage;

			if (m_HealthPoint <= 0)
			{
				m_KnockbackForce = Vector2(1.0f, 0.5f);
				m_KnockbackForce.Normalize();
				m_KnockbackForce *= 5.0f;

				if (!right)
				{
					m_KnockbackForce.x *= -1.0f;
					m_FacingRight = true;
				}
				else
					m_FacingRight = false;

				PushState(EEnemyState::KNOCKDOWN, m_CurrentTime + 0.2f);
			}
		}
		else if (type == EAttackType::KNOCKDOWN)
		{
			m_KnockbackForce = dir * force;

			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
			}
			else
				m_FacingRight = false;

			m_Sprite->ChangeAnimation("Knockdown");

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_OnGround && m_KnockbackForce.y > 0.0f)
			{
				m_Velocity.y = 0.001f;
				m_OnGround = false;
			}

			m_Physics = true;


			PushState(EEnemyState::KNOCKDOWN, m_CurrentTime + forceTime);

			if(!m_Stunned)
				m_StunAccumulate += 0.25f;

			m_HealthPoint -= damage;
		}
		else
		{
			m_KnockbackForce = dir * force;

			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
			}
			else
				m_FacingRight = false;

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_OnGround && m_KnockbackForce.y > 0.0f)
			{
				m_Velocity.y = 0.001f;
				m_OnGround = false;
			}


			m_Sprite->ChangeAnimation("Blownback");
			CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->ActiveCameraShake();
			m_Physics = true;


			PushState(EEnemyState::BLOWNBACK, m_CurrentTime + forceTime);

			if(!m_Stunned)
				m_StunAccumulate += 0.35f;

			m_HealthPoint -= damage;
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
			}
			else
				m_FacingRight = false;

			m_Sprite->ChangeAnimation("Knockdown");
				
			m_Velocity.y = 0.1f;				
			m_OnGround = false;

			m_Jump = false;

			m_Physics = true;

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			PushState(EEnemyState::KNOCKDOWN, m_CurrentTime + forceTime);

			if(!m_Stunned)
				m_StunAccumulate += 0.2f;

			m_HealthPoint -= damage;
		}
		else
		{
			m_KnockbackForce = dir * force;

			if (!right)
			{
				m_KnockbackForce.x *= -1.0f;
				m_FacingRight = true;
			}
			else
				m_FacingRight = false;

			m_Sprite->ChangeAnimation("Blownback");

			m_Velocity.y = 0.1f;
			m_OnGround = false;

			m_Physics = true;

			m_Jump = false;

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
			CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->ActiveCameraShake();
			PushState(EEnemyState::BLOWNBACK, m_CurrentTime + forceTime);

			if(!m_Stunned)
				m_StunAccumulate += 0.35f;

			m_HealthPoint -= damage;
		}
	}


	m_NavAgent->SetInterrupted(true);

	PopStateEnd(EEnemyState::ATTACK);
	PushState(EEnemyState::GETHIT);
}

void CRCGEnemy::HitStatusEnd()
{
	if (m_OnGround && !m_Stunned)
	{
		m_Sprite->ChangeAnimation("Idle");
		PopStateEnd(EEnemyState::GETHIT);
	}
	else if (m_OnGround && m_Stunned)
	{
		m_Sprite->ChangeAnimation("Stunned");
		PushState(EEnemyState::STUNNED, m_CurrentTime + 5.0f);
	}
	else
		m_Sprite->ChangeAnimation("Knockdown");

	m_HitCount = 0;
}

int CRCGEnemy::PopStateEnd(float currentTime)
{
	int finishedState = 0;

	if (m_CurrentState != (int)EEnemyState::NORMAL
		&& m_CurrentState != (int)EEnemyState::ATTACK)
	{
		int state = 1;

		for (int i = 0; i < (int)EEnemyState::MAX; i++)
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

void CRCGEnemy::PopStateEnd(EEnemyState state)
{
	m_CurrentState &= ~(int)state;
	m_StateEnd[(int)log2((int)state)] = 0.0f;
}

void CRCGEnemy::PushState(EEnemyState state, float duration)
{
	m_CurrentState |= (int)state;
	m_StateEnd[(int)log2((int)state)] = duration;
}

void CRCGEnemy::PushState(EEnemyState state)
{
	m_CurrentState |= (int)state;
}

void CRCGEnemy::PlayerDetect()
{
	m_EnemyHUD->ShowExclmationMark(true);
}

void CRCGEnemy::JumpStart()
{
	if (m_OnGround && !m_Jump)
	{
		m_Jump = true;
		m_JumpTime = m_CurrentTime;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_OnGround = false;

		m_Sprite->ChangeAnimation("Jump");
	}
}

void CRCGEnemy::AddWallCollisionCallback(CColliderBox2D* collider)
{
	collider->AddCollisionCallback<CRCGEnemy>(Collision_State::Begin, this, &CRCGEnemy::OnWallCollision);
	collider->AddCollisionCallback<CRCGEnemy>(Collision_State::End, this, &CRCGEnemy::OnWallCollisionExit);
}

void CRCGEnemy::DeleteWallCollisionCallback(CColliderBox2D* collider)
{
	collider->DeleteCollisionCallback((CGameObject*)this);
}

void CRCGEnemy::OnGround(const sCollisionResult& result)
{
	if (result.dest->GetGameObject() == this &&
		m_Velocity.y <= 0.0f)
	{
		m_OnGround = true;
		m_Jump = false;
		m_Physics = false;
		m_Push = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);

		if (CheckState(EEnemyState::ATTACK))
			PopStateEnd(EEnemyState::ATTACK);

		if (CheckState(EEnemyState::GETHIT))
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			m_HitCount = 0;
			PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
		}
		else
			m_Sprite->ChangeAnimation("Idle");
	}
}

void CRCGEnemy::OnWallCollision(const sCollisionResult& result)
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
				if (m_Physics && CheckState(EEnemyState::GETHIT) && fabs(m_KnockbackForce.x) >= 5.0f)
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
				if (m_Physics && CheckState(EEnemyState::GETHIT) && fabs(m_KnockbackForce.x) >= 5.2f)
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
}

void CRCGEnemy::OnWallCollisionExit(const sCollisionResult& result)
{
	if (result.src->GetGameObject()->GetRootComponent()->TypeCheck<CColliderBox2D>()
		&& result.dest->GetGameObject() == this)
	{
		CColliderComponent* collider = (CColliderComponent*)result.src->GetGameObject()->GetRootComponent();

		sCollisionProfile* profile = collider->GetCollisionProfile();

		if (profile->name == "SideWall")
		{
			if (result.src->GetWorldPos().x < result.dest->GetWorldPos().x)
				m_LeftWallCollision = false;
			else
				m_RightWallCollision = false;
		}
		else
		{
			if (result.src->GetWorldPos().y < result.dest->GetWorldPos().y)
				m_BottomWallCollision = false;
			else
				m_TopWallCollision = false;
		}
	}
}
