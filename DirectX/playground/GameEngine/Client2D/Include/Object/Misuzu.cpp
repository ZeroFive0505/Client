#include "Misuzu.h"
#include "MisuzuAnimation.h"
#include "RCGPlayer.h"
#include "MisuzuPursue.h"
#include "RCGPilar.h"
#include "BossEventTrigger.h"

CMisuzu::CMisuzu()
{
}

CMisuzu::CMisuzu(const CMisuzu& obj) :
	CRCGEnemy(obj)
{
}

CMisuzu::~CMisuzu()
{
	SAFE_DELETE(m_State);
	SAFE_DELETE(m_NextState);
}

bool CMisuzu::Init()
{
	if (!CRCGEnemy::Init())
		return false;

	m_Sprite->CreateAnimationInstance<CMisuzuAnimation>();

	m_Bottom->AddCollisionCallback(Collision_State::Begin, this, &CMisuzu::OnGround);

	m_NavAgent->SetMoveSpeed(m_WalkSpeed);

	SetMoveSetInfo();

	m_HitBox->AddCollisionCallback(Collision_State::Begin, this, &CMisuzu::HitBoxCheck);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Elbow", this, &CMisuzu::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction("Roar", this, &CMisuzu::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction("Slap", this, &CMisuzu::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction("WUPunch", this, &CMisuzu::GroundAttackEndCallback);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit1", this, &CRCGEnemy::HitStatusEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit2", this, &CRCGEnemy::HitStatusEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit3", this, &CRCGEnemy::HitStatusEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Getup", this, &CRCGEnemy::GetUpStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Getup", this, &CRCGEnemy::GetUpComplete);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Getup_roar", this, &CRCGEnemy::GetUpStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Getup_roar", this, &CRCGEnemy::GetUpComplete);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Knockdown_midair", this, &CRCGEnemy::ChangeToFall);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Blownback_midair", this, &CRCGEnemy::ChangeToFall);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Block_success", this, &CRCGEnemy::BlockSuccess);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Block_end", this, &CRCGEnemy::GuardOff);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Groundhit", this, &CRCGEnemy::GetGroundHit);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Groundhit", this, &CRCGEnemy::GroundHitFinish);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Knockdown_ground", this, &CRCGEnemy::ChangeToDown);

	m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Elbow", "ElbowHitBoxOn", 4, this, &CRCGEnemy::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CMisuzu>("Elbow", "ElbowSound", 4, this, &CMisuzu::ElbowBonk);
	m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Elbow", "ElbowHitBoxOff", 5, this, &CRCGEnemy::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Meteor_fall", this, &CRCGEnemy::ActivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Meteor_landhit", this, &CRCGEnemy::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Meteor_landmiss", this, &CRCGEnemy::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Slap", "SlapHitBoxOn", 7, this, &CRCGEnemy::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CMisuzu>("Slap", "Slap", 7, this, &CMisuzu::SlapSound);
	m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Slap", "SlapHitBoxOff", 8, this, &CRCGEnemy::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Tackle_loop", this, &CRCGEnemy::ActivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("WUPunch", "WUPunchHitBoxOn", 16, this, &CRCGEnemy::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("WUPunch", "WUPunchHitBoxOff", 17, this, &CRCGEnemy::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CMisuzu>("WUPunch", "WUPunchPushOn", 12, this, &CMisuzu::WUPunchPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CMisuzu>("WUPunch", "WUPunchPushOff", 17, this, &CMisuzu::WUPunchPushEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Meteor_landmiss", this, &CMisuzu::RubButt);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Rubbutt_end", this, &CMisuzu::RubButtEnd);

	m_Sprite->GetAnimationInstance()->AddNotify<CMisuzu>("Getup", "GetUpAttack", 5, this, &CMisuzu::GetUpAttack);

	m_Sprite->GetAnimationInstance()->SetStartFunction("Meteor_jumpup", this, &CMisuzu::MeteorStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction("Meteor_jumpup", this, &CMisuzu::MeteorJump);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Meteor_again", this, &CMisuzu::MeteorAgain);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Meteor_landhit", this, &CMisuzu::MeteorHit);

	m_Sprite->GetAnimationInstance()->SetStartFunction("Meteor_landmiss", this, &CMisuzu::MeteorMiss);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Tackle_start", this, &CMisuzu::TackleStart);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Tackle_turn", this, &CMisuzu::TackleTurn);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Roar", this, &CMisuzu::RoadEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction("Roar", this, &CMisuzu::RoarStart);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Getup_roar", this, &CMisuzu::GetupRoadEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction("Defeat", this, &CMisuzu::Defeat);

	m_Body->SetOffset(0.0f, 80.0f, 0.0f);

	m_Body->SetExtent(80.0f, 70.0f);

	m_ContactBox->SetExtent(60.0f, 60.0f);

	m_Bottom->SetExtent(80.0f, 10.0f);

	m_Sprite->SetWorldPos(m_Transform->GetWorldPos().x, -60.0f, m_Transform->GetWorldPos().z);
	m_Body->SetWorldPos(m_Transform->GetWorldPos().x, -60.0f, m_Transform->GetWorldPos().z);
	m_Shadow->SetRelativePos(0.0f, 0.0f, 0.0f);

	m_Shadow->SetRelativeScale(120.0f, 25.0f, 1.0f);

	m_Grabbable = false;

	m_Push = false;

	m_MeteorHit = false;

	m_Phase = EMisuzuPhase::PHASE1;

	m_MaxFallSpeed = -15.0f;

	m_Gravity = -20.0f;

	m_MeteorCnt = 0;

	m_PhaseChangeStart = false;

	m_HealthPoint = 600;

	m_MaxHealthPoint = 600;

	return true;
}

void CMisuzu::Update(float deltaTime)
{
	CRCGEnemy::Update(deltaTime);

	if (m_HitBox->IsEnable())
		HitBoxUpdate();

	if (m_Push)
	{
		if (m_FacingRight && !m_RightWallCollision)
			AddWorldPos(GetWorldAxis(AXIS_X) * 750.0f * deltaTime);
		else if (!m_FacingRight && !m_LeftWallCollision)
			AddWorldPos(GetWorldAxis(AXIS_X) * -750.0f * deltaTime);
	}

	if (m_Rubbutt)
	{
		if (m_CurrentTime > m_RubbuttEndTime)
		{
			m_Rubbutt = false;
			m_Sprite->ChangeAnimation("Rubbutt_end");
		}
	}

	if (m_MeteorFollow)
	{
		m_MeteorFollowTime += deltaTime;

		if (m_MeteorFollowTime >= 3.5f)
		{
			m_MeteorFollow = false;
			m_GravityEnable = true;
		}
	}

	if (m_TackleFollow)
	{
		m_TackleFollowTime += deltaTime;

		if (m_TackleFollowTime >= 5.0f)
		{
			m_TackleFollow = false;
			m_Sprite->ChangeAnimation("Idle");
			PopStateEnd(EEnemyState::ATTACK);
			m_CurrentMove = EMisuzuMoveSet::IDLE;
			CResourceManager::GetInst()->SoundStop("Misuzu_tackle_step");
		}
	}


	if (m_MeteorLerp)
	{
		m_MeteorJumpTime += deltaTime * 0.75f;

		if (m_MeteorJumpTime >= 1.0f)
		{
			m_MeteorLerp = false;
			m_MeteorFollowTime = 0.0f;
			m_MeteorJumpTime = 1.0f;
			m_MeteorFollow = true;
			m_MaxFallSpeed = -70.0f;
			m_Gravity = -40.0f;
			m_Sprite->ChangeAnimation("Meteor_fall");
		}

		float a = m_Body->GetWorldPos().y;
		float b = 1000.0f;

		float t = a + m_MeteorJumpTime * (b - a);

		float x = m_Body->GetWorldPos().x;

		m_Body->SetWorldPos(x, t, 0.0f);

		a = m_Sprite->GetWorldPos().y;
		b = 1000.0f;

		t = a + m_MeteorJumpTime * (b - a);

		m_Sprite->SetWorldPos(x, t, 0.0f);
	}

	if (m_CurrentMove == EMisuzuMoveSet::METEOR &&
		m_Velocity.y < 0.0f &&
		GetWorldPos().y < 800.0f)
	{
		if (!m_MeteorWarning)
		{
			CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_warning");
			m_MeteorWarning = true;
		}
	}

	if (m_CurrentMove == EMisuzuMoveSet::TACKLE)
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

		auto iter = collisions.begin();
		auto iterEnd = collisions.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter).second >= 2)
				ActivateHitBox();
			else
				DeactivateHitBox();
		}
	}

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
			if (!m_Stunned && m_PhaseChangeStart)
			{
				m_Invincible = true;
				m_Sprite->ChangeAnimation("Getup_roar");
				m_Velocity = Vector2(0.0f, 0.0f);
				m_AbsVel = Vector2(0.0f, 0.0f);
			}
			else if (m_Stunned)
			{
				m_Sprite->ChangeAnimation("Stunned");
				m_Velocity = Vector2(0.0f, 0.0f);
				m_AbsVel = Vector2(0.0f, 0.0f);
				PushState(EEnemyState::DOWN, m_CurrentTime + 3.1f);
				PushState(EEnemyState::STUNNED, m_CurrentTime + 3.0f);
			}
			else
			{
				m_Invincible = true;
				m_Sprite->ChangeAnimation("Getup");
				m_Velocity = Vector2(0.0f, 0.0f);
				m_AbsVel = Vector2(0.0f, 0.0f);
			}
		}
		else
		{
			PushState(EEnemyState::DEFEADTED);
			m_Sprite->ChangeAnimation("Defeat");
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
		if (!m_PhaseChangeStart)
		{
			m_Sprite->ChangeAnimation("Getup");
			PopStateEnd(EEnemyState::DOWN);
			m_Invincible = true;
			m_Stunned = false;
		}
		else
			m_Sprite->ChangeAnimation("Getup_roar");
	}


	if (m_FinishedState & (int)EEnemyState::DEFEADTED)
		Destroy();

	if (m_CurrentState != (int)EEnemyState::NORMAL)
	{
		if (m_State && m_State->GetCurrentState() == STATE::ATTACK)
		{
			if (m_State)
			{
				CState* state = m_State->Process();

				if (state->GetCurrentState() != m_State->GetCurrentState())
				{
					SAFE_DELETE(m_State);
					m_State = state;
				}
			}

			return;
		}
		else
			return;
	}

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

void CMisuzu::PostUpdate(float deltaTime)
{
	CRCGEnemy::PostUpdate(deltaTime);
}

CMisuzu* CMisuzu::Clone()
{
	return new CMisuzu(*this);
}

void CMisuzu::SetMoveSetInfo()
{
	sEnemyMove info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.0f, 0.3f);
	info.forceDir.Normalize();
	info.force = 14.0f;
	info.damage = 8;
	info.forceTime = 0.4f;

	m_mapMoveset[EMisuzuMoveSet::ELBOW] = info;

	info = {};

	info.attackType = EAttackType::BLOWNBACK;
	info.forceDir = Vector2(1.0f, 0.15f);
	info.forceDir.Normalize();
	info.force = 16.0f;
	info.damage = 15;
	info.forceTime = 0.4f;

	m_mapMoveset[EMisuzuMoveSet::WUPUNCH] = info;

	info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.0f, 0.25f);
	info.forceDir.Normalize();
	info.force = 10.0f;
	info.damage = 10;
	info.forceTime = 0.4f;

	m_mapMoveset[EMisuzuMoveSet::SLAP] = info;

	info = {};

	info.attackType = EAttackType::BLOWNBACK;
	info.forceDir = Vector2(1.0f, 0.5f);
	info.forceDir.Normalize();
	info.force = 16.0f;
	info.damage = 12;
	info.forceTime = 0.4f;

	m_mapMoveset[EMisuzuMoveSet::TACKLE] = info;

	info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.5f, 0.7f);
	info.forceDir.Normalize();
	info.force = 10.0f;
	info.damage = 5;
	info.forceTime = 0.4f;

	m_mapMoveset[EMisuzuMoveSet::ROAR] = info;

	info = {};

	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.0f, 0.7f);
	info.forceDir.Normalize();
	info.force = 14.0f;
	info.damage = 15;
	info.forceTime = 0.5f;

	m_mapMoveset[EMisuzuMoveSet::METEOR] = info;
}

void CMisuzu::RoarStart()
{
	CResourceManager::GetInst()->SoundPlay("Misuzu_fury");

	if (m_Phase == EMisuzuPhase::PHASE1)
	{
		m_Phase = EMisuzuPhase::PHASE2;
		m_HealthPoint = 400;
		m_Sprite->SetBaseColor(0.8f, 0.0f, 0.0f, 1.0f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Idle", 1.1f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Walk", 1.1f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Slap", 1.1f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Elbow", 1.1f);
		m_WalkSpeed = 250.0f * 1.2f;
	}
	else if (m_Phase == EMisuzuPhase::PHASE2)
	{
		m_Phase = EMisuzuPhase::PHASE3;
		m_HealthPoint = 200;
		m_Sprite->SetBaseColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Idle", 1.25f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Walk", 1.25f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Slap", 1.25f);
		m_Sprite->GetAnimationInstance()->SetPlayScale("Elbow", 1.25f);
		m_WalkSpeed = 250.0f * 1.5f;
	}



	CRCGPlayer* player = (CRCGPlayer*)m_Scene->GetPlayerObject();

	if (GetWorldPos().SqrDistance(player->GetWorldPos()) <= 40000.0f)
	{
		bool right;

		if (player->GetWorldPos().x > GetWorldPos().x)
			right = true;
		else
			right = false;

		player->GetHit(EAttackType::KNOCKDOWN, Vector2(1.0f, 0.3f), 0, 7.0f, 0.5f, right);
	}

	m_Trigger->SetHealthBar(m_HealthPoint / (float)m_MaxHealthPoint);

	m_PhaseChangeStart = false;
}

void CMisuzu::SlapSound()
{
	int randomSoundIndex = (rand() % 4) + 1;

	std::string name = "Misuzu_effort" + std::to_string(randomSoundIndex);

	CResourceManager::GetInst()->SoundPlay(name);

	CResourceManager::GetInst()->SoundPlay("Misuzu_slap");
}

void CMisuzu::ElbowBonk()
{
	int randomSoundIndex = (rand() % 4) + 1;

	std::string name = "Misuzu_effort" + std::to_string(randomSoundIndex);

	CResourceManager::GetInst()->SoundPlay(name);

	CResourceManager::GetInst()->SoundPlay("Misuzu_elbow_bonk");
}

void CMisuzu::GetUpAttack()
{
	CRCGPlayer* player = (CRCGPlayer*)m_Scene->GetPlayerObject();

	CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_impact");

	if (GetWorldPos().Distance(player->GetWorldPos()) <= 150.0f)
	{
		bool right;

		if (player->GetWorldPos().x > GetWorldPos().x)
			right = true;
		else
			right = false;

		player->GetHit(EAttackType::KNOCKDOWN, Vector2(1.0f, 0.3f), 5, 18.0f, 0.4f, right);
	}
}

void CMisuzu::AddTackleCollision(CColliderBox2D* collider)
{
	collider->AddCollisionCallback<CMisuzu>(Collision_State::Begin, this, &CMisuzu::OnPilarBottomCollision);
}

void CMisuzu::OnPilarBottomCollision(const sCollisionResult& result)
{
	if (m_CurrentMove == EMisuzuMoveSet::TACKLE)
	{
		if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
		{
			bool right;

			if (result.src->GetGameObject()->GetWorldPos().x < result.dest->GetGameObject()->GetWorldPos().x)
				right = true;
			else
				right = false;

			m_CurrentMove = EMisuzuMoveSet::IDLE;

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			Vector2 dir = Vector2(1.0f, 0.25f);
			dir.Normalize();
			GetHit(EAttackType::KNOCKDOWN, dir, 0, 12.0f, 0.3f, right);

			CRCGPilar* pilar = (CRCGPilar*)result.src->GetGameObject();
			m_Stunned = true;

			m_TackleFollow = false;

			CResourceManager::GetInst()->SoundStop("Misuzu_tackle_step");

			pilar->ResetTile();
			pilar->Destroy();
		}

	}
	else if (m_Physics)
	{
		if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
		{
			m_Physics = false;
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
		}
	}
}

void CMisuzu::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
	if (m_Invincible)
		return;

	m_Push = false;

	m_HitBox->Enable(false);

	if (m_Rubbutt)
		m_Rubbutt = false;

	switch (type)
	{
	case EAttackType::HIT:
	case EAttackType::KNOCKDOWN:
	{
		int randomSoundIndex = (rand() % 6) + 1;

		std::string name = "Misuzu_gethit" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
	}
	break;
	case EAttackType::BLOWNBACK:
	{
		int randomSoundIndex = (rand() % 2) + 1;

		std::string name = "Misuzu_gethit_heavy" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
	}
	break;
	}

	if (m_CurrentMove == EMisuzuMoveSet::TACKLE)
	{
		m_CurrentMove = EMisuzuMoveSet::IDLE;
		DeactivateHitBox();
		m_Sprite->ChangeAnimation("Idle");
		CResourceManager::GetInst()->SoundStop("Misuzu_tackle_step");
	}


	if (m_OnGround)
	{
		if (type == EAttackType::HIT)
		{
			if (!CheckState(EEnemyState::DOWN))
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
			else
				m_Sprite->ChangeAnimation("Groundhit");

			m_HealthPoint -= damage;


			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			if (m_HealthPoint <= 0)
			{
				m_KnockbackForce = Vector2(1.0f, 0.5f);
				m_KnockbackForce.Normalize();
				m_KnockbackForce *= 4.5f;

				if (!right)
				{
					m_KnockbackForce.x *= -1.0f;
					m_FacingRight = true;
				}
				else
					m_FacingRight = false;

				PushState(EEnemyState::KNOCKDOWN, m_CurrentTime + 0.2f);
			}

			if (m_HealthPoint <= 400 && m_Phase == EMisuzuPhase::PHASE1)
			{
				if (!CheckState(EEnemyState::DOWN))
					PhaseChange();
				else
					PhaseChangeStart();
			}
			else if (m_HealthPoint <= 200 && m_Phase == EMisuzuPhase::PHASE2)
			{
				if (!CheckState(EEnemyState::DOWN))
					PhaseChange();
				else
					PhaseChangeStart();
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

			m_HealthPoint -= damage;

			if (m_HealthPoint <= 400 && m_Phase == EMisuzuPhase::PHASE1)
				PhaseChange();
			else if (m_HealthPoint <= 200 && m_Phase == EMisuzuPhase::PHASE2)
				PhaseChange();
			else
			{
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
			}
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

			m_HealthPoint -= damage;

			if (m_HealthPoint <= 400 && m_Phase == EMisuzuPhase::PHASE1)
				PhaseChange();
			else if (m_HealthPoint <= 200 && m_Phase == EMisuzuPhase::PHASE2)
				PhaseChange();
			else
			{

				m_Sprite->ChangeAnimation("Blownback");

				m_Velocity = Vector2(0.0f, 0.0f);
				m_AbsVel = Vector2(0.0f, 0.0f);

				CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->ActiveCameraShake();
				m_Physics = true;

				if (m_OnGround && m_KnockbackForce.y > 0.0f)
				{
					m_Velocity.y = 0.001f;
					m_OnGround = false;
				}

				PushState(EEnemyState::BLOWNBACK, m_CurrentTime + forceTime);
			}
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

			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);

			m_Velocity.y = 1.0f;

			m_Physics = true;


			PushState(EEnemyState::KNOCKDOWN, m_CurrentTime + forceTime);

			m_HealthPoint -= damage;

			if (m_HealthPoint <= 400 && m_Phase == EMisuzuPhase::PHASE1)
				PhaseChangeStart();
			else if (m_HealthPoint <= 200 && m_Phase == EMisuzuPhase::PHASE2)
				PhaseChangeStart();
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

			m_Velocity.y = 1.0f;

			m_Sprite->ChangeAnimation("Blownback");

			m_Physics = true;

			CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->ActiveCameraShake();
			PushState(EEnemyState::BLOWNBACK, m_CurrentTime + forceTime);

			m_HealthPoint -= damage;

			if (m_HealthPoint <= 400 && m_Phase == EMisuzuPhase::PHASE1)
				PhaseChangeStart();
			else if (m_HealthPoint <= 200 && m_Phase == EMisuzuPhase::PHASE2)
				PhaseChangeStart();
		}
	}

	m_NavAgent->SetInterrupted(true);

	m_HealthPoint = max(0, m_HealthPoint);

	m_Trigger->SetHealthBar(m_HealthPoint / (float)m_MaxHealthPoint);

	if (m_HealthPoint <= 0 && !CEngine::GetInst()->IsSlowMotion())
	{
		CResourceManager::GetInst()->SoundPlay("Boss_finalhit");
		m_Sprite->SetBaseColor(1.0f, 1.0f, 1.0f, 1.0f);
		CEngine::GetInst()->SetSlowMotion(3.0f, 0.75f);
	}

	PopStateEnd(EEnemyState::ATTACK);
	PushState(EEnemyState::GETHIT);
}

void CMisuzu::HitBoxUpdate()
{
	Vector3 size = m_Sprite->GetRelativeScale() * 0.5f;
	m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());

	switch (m_CurrentMove)
	{
	case EMisuzuMoveSet::ELBOW:
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
		break;
	case EMisuzuMoveSet::METEOR:
		m_HitBox->SetExtent(size.x * 0.5f, size.y);
		break;
	case EMisuzuMoveSet::SLAP:
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
		break;
	case EMisuzuMoveSet::TACKLE:
		m_HitBox->SetExtent(size.x * 0.75f, size.y * 0.5f);
		break;
	case EMisuzuMoveSet::WUPUNCH:
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
		break;
	}
}

void CMisuzu::HitBoxCheck(const sCollisionResult& result)
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

				if (player->OnGuard() && m_CurrentMove != EMisuzuMoveSet::TACKLE)
				{
					/*  if (m_CurrentTime - player->GetGuardStartTime() <= 1.5f)
					  {
						  PushState(EEnemyState::PARALYSIS, m_CurrentTime + 1.0f);
						  m_Sprite->ChangeAnimation("Hit1");
						  player->Parry();
					  }
					  else*/
					player->Block();
					CResourceManager::GetInst()->SoundPlay("Block");
				}
				else
				{
					player->GetHit(move.attackType, move.forceDir, move.damage, move.force, move.forceTime, right);

					if (m_CurrentMove == EMisuzuMoveSet::METEOR)
						m_MeteorHit = true;

					if (m_CurrentMove == EMisuzuMoveSet::TACKLE)
					{
						m_CurrentMove = EMisuzuMoveSet::IDLE;
						PopStateEnd(EEnemyState::ATTACK);
						DeactivateHitBox();
						m_Sprite->ChangeAnimation("Idle");
						CResourceManager::GetInst()->SoundStop("Misuzu_tackle_step");
					}

					if (m_CurrentMove == EMisuzuMoveSet::SLAP)
						CResourceManager::GetInst()->SoundPlay("Misuzu_slap_impact");

					CResourceManager::GetInst()->SoundPlay("Knockdown");
				}
			}
		}
	}
}

void CMisuzu::OnGround(const sCollisionResult& result)
{
	if (result.dest->GetGameObject() == this &&
		m_Velocity.y <= 0.0f)
	{
		m_OnGround = true;
		m_Jump = false;
		m_Physics = false;
		m_Push = false;
		m_MeteorFollow = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);

		m_MaxFallSpeed = -15.0f;
		m_Gravity = -20.0f;


		if (CheckState(EEnemyState::ATTACK))
			PopStateEnd(EEnemyState::ATTACK);


		if (CheckState(EEnemyState::GETHIT))
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			m_HitCount = 0;
			if (m_Stunned)
				PushState(EEnemyState::DOWN, m_CurrentTime + 1.0f);
			else if (m_PhaseChangeStart)
				PushState(EEnemyState::DOWN, m_CurrentTime + 2.0f);
			else
				PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
		}
		else
		{
			if (m_CurrentMove == EMisuzuMoveSet::METEOR && m_MeteorHit)
			{
				m_Sprite->ChangeAnimation("Meteor_landhit");
				PushState(EEnemyState::ATTACK);
				m_Invincible = false;
				CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_impact");
			}
			else if (m_CurrentMove == EMisuzuMoveSet::METEOR && !m_MeteorHit)
			{
				switch (m_Phase)
				{
				case EMisuzuPhase::PHASE1:
				case EMisuzuPhase::PHASE2:
					m_Sprite->ChangeAnimation("Meteor_landmiss");
					m_Invincible = false;
					m_RubbuttEndTime = m_CurrentTime + 3.0f;
					m_Rubbutt = true;
					CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_impact");
					break;
				case EMisuzuPhase::PHASE3:
					m_MeteorCnt++;
					if (m_MeteorCnt < 3)
					{
						m_Sprite->ChangeAnimation("Meteor_again");
						CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_impact");
					}
					else
					{
						m_Sprite->ChangeAnimation("Meteor_landmiss");
						CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_impact");
						m_Invincible = false;
						m_RubbuttEndTime = m_CurrentTime + 3.0f;
						m_Rubbutt = true;
					}
					break;
				}

				std::vector<CRCGPilar*> pilars = m_Trigger->GetPilars();

				size_t size = pilars.size();

				if ((int)size != 0)
				{
					CRCGPilar* closestPilar = pilars.front();
					for (size_t i = 0; i < size; i++)
					{
						if (m_Bottom->GetWorldPos().Distance(pilars[i]->GetPilarBottomCollider()->GetWorldPos()) <
							m_Bottom->GetWorldPos().Distance(closestPilar->GetPilarBottomCollider()->GetWorldPos()))
							closestPilar = pilars[i];
					}

					if (closestPilar->GetWorldPos().Distance(m_Bottom->GetWorldPos()) <= 200.0f)
					{
						closestPilar->ResetTile();
						closestPilar->Destroy();
					}
				}
			}
		}
	}
}

void CMisuzu::OnWallCollision(const sCollisionResult& result)
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
				m_AbsVel.x = 0.0f;
				m_Velocity.x = 0.0f;
				m_LeftWallCollision = true;
			}
			else
			{
				m_AbsVel.x = 0.0f;
				m_Velocity.x = 0.0f;
				m_RightWallCollision = true;
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

void CMisuzu::PursueStart()
{
	SAFE_DELETE(m_State);

	m_State = (CState*)new CMisuzuPursue(this, m_NextState);
}
