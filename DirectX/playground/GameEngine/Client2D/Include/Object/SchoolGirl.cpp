#include "SchoolGirl.h"
#include "SchoolGirlAnimation.h"
#include "../Widget/RCGEnemyHUD.h"
#include "SchoolGirlIdle.h"
#include "SchoolGirlPursue.h"

CSchoolGirl::CSchoolGirl()
{
}

CSchoolGirl::CSchoolGirl(const CSchoolGirl& obj) :
	CRCGEnemy(obj)
{
}

CSchoolGirl::~CSchoolGirl()
{
	SAFE_DELETE(m_State);
	SAFE_DELETE(m_NextState);
}

bool CSchoolGirl::Init()
{
	if (!CRCGEnemy::Init())
		return false;

	m_Sprite->CreateAnimationInstance<CSchoolGirlAnimation>();


	m_Bottom->AddCollisionCallback<CRCGEnemy>(Collision_State::Begin, this, &CRCGEnemy::OnGround);

	m_NavAgent->SetMoveSpeed(m_WalkSpeed);

	SetMoveSetInfo();

	m_HitBox->AddCollisionCallback<CSchoolGirl>(Collision_State::Begin, this, &CSchoolGirl::HitBoxCheck);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolGirl>("Jab", this, &CSchoolGirl::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolGirl>("Kick", this, &CSchoolGirl::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolGirl>("Cresentkick", this, &CSchoolGirl::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolGirl>("Jumpknee", this, &CSchoolGirl::AirAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolGirl>("Tornadokick_start", this, &CSchoolGirl::TornadoKickStart);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit1", this, &CRCGEnemy::HitStatusEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit2", this, &CRCGEnemy::HitStatusEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit3", this, &CRCGEnemy::HitStatusEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Getup", this, &CRCGEnemy::GetUpStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Getup", this, &CRCGEnemy::GetUpComplete);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Knockdown_midair", this, &CRCGEnemy::ChangeToFall);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Blownback_midair", this, &CRCGEnemy::ChangeToFall);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Block_success", this, &CRCGEnemy::BlockSuccess);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Block_end", this, &CRCGEnemy::GuardOff);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Groundhit", this, &CRCGEnemy::GetGroundHit);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Groundhit", this, &CRCGEnemy::GroundHitFinish);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Knockdown_ground", this, &CRCGEnemy::ChangeToDown);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Heldrelease", this, &CRCGEnemy::GrabbedReleaseEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Heldhit", this, &CRCGEnemy::GutPunchEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Wallhit", this, &CRCGEnemy::WallHitStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Wallhit", this, &CRCGEnemy::WallHitEnd);

	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Cresentkick", "CresentkickHitBoxOn1", 3, this, &CSchoolGirl::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Cresentkick", "CresentkickHitBoxOff1", 4, this, &CSchoolGirl::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Cresentkick", "CresentkickHitBoxOn2", 6, this, &CSchoolGirl::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Cresentkick", "CresentkickHitBoxOff2", 7, this, &CSchoolGirl::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Jab", "JabHitBoxOn", 2, this, &CSchoolGirl::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Jab", "JabHitBoxOff", 3, this, &CSchoolGirl::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Jumpknee", "JumpkneeHitBoxOn", 2, this, &CSchoolGirl::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Jumpknee", "JumpkneeHitBoxOff", 3, this, &CSchoolGirl::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Kick", "KickHitBoxOn", 2, this, &CSchoolGirl::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CSchoolGirl>("Kick", "KickHitBoxOff", 3, this, &CSchoolGirl::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CSchoolGirl>("Tornadokick_loop", this, &CSchoolGirl::ActivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CSchoolGirl>("Tornadokick_end", this, &CSchoolGirl::TornadoKickEnd);

	m_Push = false;

	m_HUDWidget = CreateComponent<CWidgetComponent>("EnemyHUD");

	m_EnemyHUD = m_HUDWidget->CreateWidgetWindow<CRCGEnemyHUD>("EnemyHUDWidget");

	m_Transform->AddChild(m_HUDWidget);

	m_HUDWidget->SetRelativePos(-22.5f, 80.0f, 0.0f);

	m_Grabbable = true;

	return true;
}

void CSchoolGirl::Update(float deltaTime)
{
	CRCGEnemy::Update(deltaTime);

	if (m_Spawning)
		return;

	if (m_CurrentState == (int)EEnemyState::NORMAL && !m_Physics)
		m_StunAccumulate -= deltaTime;

	if (m_StunAccumulate <= 0.0f)
		m_StunAccumulate = 0.0f;


	if ((m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Knockdown_fall") ||
		m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Knockdown")) && m_OnGround)
	{
		m_Sprite->ChangeAnimation("Knockdown_ground");
		PopStateEnd(EEnemyState::GETHIT);
		m_HitCount = 0;
		m_Physics = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
		PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
	}

	m_FinishedState = PopStateEnd(m_CurrentTime);

	if (m_FinishedState & (int)EEnemyState::DOWN)
	{
		if (m_HealthPoint > 0)
		{
			m_Invincible = true;
			m_Sprite->ChangeAnimation("Getup");
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
		}
		else
		{
			m_BlinkTime = 0.0f;
			m_HalfOpacity = true;
			PushState(EEnemyState::DEFEADTED, m_CurrentTime + 3.0f);
		}
	}

	if (m_FinishedState & (int)EEnemyState::KNOCKDOWN)
	{
		if (!m_OnGround)
			m_Sprite->ChangeAnimation("Knockdown_midair");
		else
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			m_Physics = false;
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
			PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
		}
	}

	if (m_FinishedState & (int)EEnemyState::BLOWNBACK)
	{
		if (!m_OnGround)
			m_Sprite->ChangeAnimation("Blownback_midair");
		else
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			m_Physics = false;
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
			PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
		}
	}

	if (m_FinishedState & (int)EEnemyState::STUNNED)
	{
		m_Sprite->ChangeAnimation("Idle");
		PopStateEnd(EEnemyState::GETHIT);
		m_Stunned = false;
	}

	if (m_FinishedState & (int)EEnemyState::GRABBED)
	{
		m_Sprite->ChangeAnimation("Heldrelease");
		if (m_Player)
		{
			((CRCGPlayer*)m_Player)->GrabbingFinish();
			m_Player = nullptr;
		}
	}

	if (m_FinishedState & (int)EEnemyState::PARALYSIS)
	{
		PopStateEnd(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Idle");
	}


	if (m_FinishedState & (int)EEnemyState::DEFEADTED)
		Destroy();

	if (m_HitBox->IsEnable())
		HitBoxUpdate();

	if (m_Push)
	{
		if(m_FacingRight && !m_RightWallCollision)
			AddWorldPos(GetWorldAxis(AXIS_X) * 200.0f * deltaTime);
		else if(!m_FacingRight && !m_LeftWallCollision)
			AddWorldPos(GetWorldAxis(AXIS_X) * -200.0f * deltaTime);
	}

	if (!m_OnGround && m_Velocity.y <= 0.0f && m_CurrentMove == ESchoolGirlMoveSet::TORNATOKICK)
	{
		if (m_Body->GetWorldPos().SqrDistance(m_Bottom->GetWorldPos()) <= 30000.0f)
		{
			m_Sprite->ChangeAnimation("Tornadokick_end");
		}
	}

	if (CheckState(EEnemyState::DEFEADTED))
	{
		if (m_HalfOpacity)
			m_Sprite->SetOpacity(0.5f);
		else
			m_Sprite->SetOpacity(1.0f);

		m_BlinkTime += deltaTime;

		if (m_BlinkTime >= 0.25f)
			m_HalfOpacity = !m_HalfOpacity;
	}

	if (m_CurrentState != (int)EEnemyState::NORMAL)
		return;

	if (m_State)
	{
		CState* state = m_State->Process();

		if (state->GetCurrentState() != m_State->GetCurrentState())
		{
			SAFE_DELETE(m_State);
			m_State = state;
		}
	}
}

void CSchoolGirl::PostUpdate(float deltaTime)
{
	CRCGEnemy::PostUpdate(deltaTime);
}

CSchoolGirl* CSchoolGirl::Clone()
{
	return new CSchoolGirl(*this);
}

void CSchoolGirl::SetMoveSetInfo()
{
	sEnemyMove info = {};

	info.attackType = EAttackType::HIT;
	info.forceDir = Vector2(1.0f, 0.1f);
	info.forceDir.Normalize();
	info.force = 4.0f;
	info.damage = 2;
	info.forceTime = 0.15f;

	m_mapMoveset[ESchoolGirlMoveSet::JAB] = info;

	info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.0f, 0.25f);
	info.forceDir.Normalize();
	info.force = 15.0f;
	info.damage = 8;
	info.forceTime = 0.4f;

	m_mapMoveset[ESchoolGirlMoveSet::CRESENTKICK] = info;

	info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.0f, 0.25f);
	info.forceDir.Normalize();
	info.force = 12.0f;
	info.damage = 4;
	info.forceTime = 0.35f;
	
	m_mapMoveset[ESchoolGirlMoveSet::JUMPKNEE] = info;

	info = {};

	info.attackType = EAttackType::BLOWNBACK;
	info.forceDir = Vector2(1.0f, 0.2f);
	info.forceDir.Normalize();
	info.forceTime = 0.4f;
	info.damage = 7;
	info.force = 18.0f;

	m_mapMoveset[ESchoolGirlMoveSet::KICK] = info;

	info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.0f, 0.5f);
	info.forceDir.Normalize();
	info.damage = 8;
	info.force = 15.0f;
	info.forceTime = 0.4f;

	m_mapMoveset[ESchoolGirlMoveSet::TORNATOKICK] = info;
}

void CSchoolGirl::HitBoxUpdate()
{
	Vector3 size = m_Sprite->GetRelativeScale() * 0.5f;
	m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());

	switch (m_CurrentMove)
	{
	case ESchoolGirlMoveSet::JAB:
	case ESchoolGirlMoveSet::KICK:
	case ESchoolGirlMoveSet::JUMPKNEE:
	{
		if (m_FacingRight)
		{
			m_HitBox->SetExtent(size.x * 0.5f, size.y);
			m_HitBox->SetOffset(size.x * 0.5f, 0.0f, 0.0f);
		}
		else
		{
			m_HitBox->SetExtent(size.x * 0.5f, size.y);
			m_HitBox->SetOffset(-size.x * 0.5f, 0.0f, 0.0f);
		}
	}
	break;
	case ESchoolGirlMoveSet::CRESENTKICK:
	case ESchoolGirlMoveSet::TORNATOKICK:
	{
		if (m_FacingRight)
		{
			m_HitBox->SetExtent(size.x * 0.75f, size.y);
			m_HitBox->SetOffset(size.x * 0.5f, 0.0f, 0.0f);
		}
		else
		{
			m_HitBox->SetExtent(size.x * 0.75f, size.y);
			m_HitBox->SetOffset(-size.x * 0.5f, 0.0f, 0.0f);
		}
	}
	break;
	}
}

void CSchoolGirl::HitBoxCheck(const sCollisionResult& result)
{
	if (!m_HitBox->IsEnable())
		return;

	if (result.dest->GetCollisionProfile()->name == "Player")
	{
		std::unordered_map<CGameObject*, int> collisions;

		{
			auto iter = m_Body->GetPrevColliderList().cbegin();
			auto iterEnd = m_Body->GetPrevColliderList().cend();

			for (; iter != iterEnd; iter++)
			{
				if ((*iter)->GetCollisionProfile()->name == "Player")
				{
					collisions[(*iter)->GetGameObject()]++;
				}
			}
		}

		{
			auto iter = m_Bottom->GetPrevColliderList().cbegin();
			auto iterEnd = m_Bottom->GetPrevColliderList().cend();

			for (; iter != iterEnd; iter++)
			{
				if ((*iter)->GetCollisionProfile()->name == "PlayerBottom")
				{
					collisions[(*iter)->GetGameObject()]++;
				}
			}
		}


		if (!collisions.empty())
		{
			sEnemyMove move = m_mapMoveset[m_CurrentMove];

			auto iter = collisions.begin();
			auto iterEnd = collisions.end();

			for (; iter != iterEnd; iter++)
			{
				bool right;

				if (GetWorldPos().x < (*iter).first->GetWorldPos().x)
					right = true;
				else
					right = false;

				CRCGPlayer* player = ((CRCGPlayer*)(*iter).first);

				if (player->IsInvincible() || player->CheckState(EKyokoState::INVINCIBLE))
					return;

				if ((*iter).second < 2)
					continue;

				if (player->CheckState(EKyokoState::DOWN))
					return;

				if (player->OnGuard())
				{
					if (m_CurrentTime - player->GetGuardStartTime() <= 1.7f)
					{
						PushState(EEnemyState::PARALYSIS, m_CurrentTime + 1.0f);
						m_Sprite->ChangeAnimation("Hit1");
						CResourceManager::GetInst()->SoundPlay("Parry");
						DeactivateHitBox();
						player->Parry();
					}
					else
					{
						player->Block();
						CResourceManager::GetInst()->SoundPlay("Block");
					}
				}
				else
				{
					player->GetHit(move.attackType, move.forceDir, move.damage, move.force, move.forceTime, right);
					CResourceManager::GetInst()->SoundPlay("Knockdown");
				}
			}
		}
	}
}

void CSchoolGirl::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
	if (m_CurrentMove == ESchoolGirlMoveSet::TORNATOKICK)
		m_CurrentMove = ESchoolGirlMoveSet::IDLE;

	CRCGEnemy::GetHit(type, dir, damage, force, forceTime, right);
}

void CSchoolGirl::SetIdleState()
{
	SAFE_DELETE(m_State);

	m_State = (CState*)new CSchoolGirlIdle(this, m_NextState);
}

void CSchoolGirl::SetPursueState()
{
	SAFE_DELETE(m_State);

	m_State = (CState*)new CSchoolGirlPursue(this, m_NextState);
}
