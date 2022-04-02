#pragma once

#include "RCGEnemy.h"
#include "State.h"

enum class EMisuzuMoveSet
{
	IDLE,
	ELBOW,
	ROAR,
	METEOR,
	SLAP,
	TACKLE,
	WUPUNCH,
	MAX
};

enum class EMisuzuPhase
{
	PHASE1,
	PHASE2,
	PHASE3
};

class CMisuzu :
	public CRCGEnemy
{
	friend class CScene;

protected:
	CMisuzu();
	CMisuzu(const CMisuzu& obj);
	virtual ~CMisuzu();

private:
	Vector3 m_Offset;
	std::unordered_map<EMisuzuMoveSet, sEnemyMove> m_mapMoveset;
	EMisuzuMoveSet m_CurrentMove;
	EMisuzuPhase m_Phase;
	CState* m_State;
	CState* m_NextState;

	bool m_MeteorHit;
	bool m_MeteorLerp;
	bool m_MeteorFollow;
	bool m_TackleFollow;
	bool m_Rubbutt;
	bool m_PhaseChangeStart;

	float m_RubbuttEndTime;
	float m_MeteorFollowTime;
	float m_TackleFollowTime;
	float m_MeteorJumpTime;

	int m_MeteorCnt;

	bool m_MeteorWarning;


	int m_MaxHealthPoint;
	Vector3 m_TackleDirection;
	class CBossEventTrigger* m_Trigger;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMisuzu* Clone();

private:
	void SetMoveSetInfo();

public:
	inline EMisuzuPhase GetMisuzuPhase() const
	{
		return m_Phase;
	}

	inline EMisuzuMoveSet GetCurrentMove() const
	{
		return m_CurrentMove;
	}

	inline float GetMeteorFollowTime() const
	{
		return m_MeteorFollowTime;
	}

	inline bool IsMeteorFollowing() const
	{
		return m_MeteorFollow;
	}

	inline bool IsTackleFollowing() const
	{
		return m_TackleFollow;
	}

	inline void SetTackleDirection(const Vector3& dir)
	{
		m_TackleDirection = dir;
		m_TackleDirection.Normalize();
	}

	inline Vector3 GetTackleDirection() const
	{
		return m_TackleDirection;
	}

	inline virtual void GetUpComplete()
	{
		m_Sprite->ChangeAnimation("Idle");
		PopStateEnd(EEnemyState::GETHIT);
		m_Invincible = false;
	}

public:
	inline void Elbow()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Elbow");
		m_CurrentMove = EMisuzuMoveSet::ELBOW;
	}

	inline void Roar()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Roar");
		m_CurrentMove = EMisuzuMoveSet::ROAR;
	}

	inline void Meteor()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Meteor_jumpup");
		m_MeteorCnt = 0;
		m_CurrentMove = EMisuzuMoveSet::METEOR;
	}

	inline void Slap()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Slap");
		m_CurrentMove = EMisuzuMoveSet::SLAP;
	}

	inline void Tackle()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Tackle_start");
		CGameObject* player = m_Scene->GetPlayerObject();
		m_TackleDirection = player->GetWorldPos() - GetWorldPos();
		m_TackleDirection.Normalize();
		m_CurrentMove = EMisuzuMoveSet::TACKLE;
	}

	inline void WUPunch()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("WUPunch");
		int randomSoundIndex = (rand() % 2) + 1;

		std::string name = "Misuzu_big_effort" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
		CResourceManager::GetInst()->SoundPlay("Misuzu_windup_punch");

		m_CurrentMove = EMisuzuMoveSet::WUPUNCH;
	}

	inline void TackleStart()
	{
		m_Sprite->ChangeAnimation("Tackle_loop");
		CResourceManager::GetInst()->SoundPlay("Misuzu_tackle_step");
		m_TackleFollow = true;
		m_TackleFollowTime = 0.0f;
	}

	inline void TackleTurn()
	{
		if (m_FacingRight)
			SetFacingDirection(false);
		else
			SetFacingDirection(true);

		m_Sprite->ChangeAnimation("Tackle_loop");
	}

	inline void MeteorJump()
	{
		m_OnGround = false;
		m_GravityEnable = false;
		m_MeteorLerp = true;
		m_MeteorJumpTime = 0.0f;
		m_Velocity.y = 0.0001f;
		m_MeteorWarning = false;
		m_Sprite->ChangeAnimation("Meteor_jump");
	}

	inline void RubButt()
	{
		m_Sprite->ChangeAnimation("Rubbutt");
	}

	inline void RubButtEnd()
	{
		PopStateEnd(EEnemyState::GETHIT);
		m_Sprite->ChangeAnimation("Idle");
	}

	inline void Guard()
	{
		m_Sprite->ChangeAnimation("Guard");
		m_Guard = true;
	}

	inline void GuardEnd()
	{
		m_Sprite->ChangeAnimation("Idle");
		m_Guard = false;
	}

	inline void GroundAttackEndCallback()
	{
		PopStateEnd(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Idle");
		m_CurrentMove = EMisuzuMoveSet::IDLE;

		if (m_Invincible)
			m_Invincible = false;

		DeactivateHitBox();
	}

	inline void MeteorStart()
	{
		m_Invincible = true;
	}

	inline void MeteorMiss()
	{
		PushState(EEnemyState::GETHIT);
		m_CurrentMove = EMisuzuMoveSet::IDLE;
		DeactivateHitBox();
	}

	inline void MeteorAgain()
	{
		m_Sprite->ChangeAnimation("Meteor_jumpup");
	}

	inline void MeteorHit()
	{
		PopStateEnd(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Idle");
		m_CurrentMove = EMisuzuMoveSet::IDLE;
	}

	inline void WUPunchPush()
	{
		m_Push = true;
	}

	inline void WUPunchPushEnd()
	{
		m_Push = false;
	}

	void RoarStart();
	
	inline void PhaseChange()
	{
		m_Invincible = true;
		m_Sprite->ChangeAnimation("Roar");
	}

	inline void PhaseChangeStart()
	{
		m_Invincible = true;
		m_PhaseChangeStart = true;
	}

	inline void GetupRoadEnd()
	{
		m_PhaseChangeStart = false;
		m_Sprite->ChangeAnimation("Roar");
	}

	inline void RoadEnd()
	{
		PopStateEnd(EEnemyState::GETHIT);
		m_Invincible = false;
		m_Sprite->ChangeAnimation("Idle");
	}

	inline void Defeat()
	{
		m_Sprite->ChangeAnimation("Defeat_loop");
	}

	void SlapSound();

	void ElbowBonk();

public:
	void GetUpAttack();

	void AddTackleCollision(CColliderBox2D* collider);
	void OnPilarBottomCollision(const sCollisionResult& result);

	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right);

	inline void SetTrigger(class CBossEventTrigger* trigger)
	{
		m_Trigger = trigger;
	}

	inline class CBossEventTrigger* GetTrigger() const
	{
		return m_Trigger;
	}

public:
	void HitBoxUpdate();
	void HitBoxCheck(const sCollisionResult& result);

	virtual void OnGround(const sCollisionResult& result);
	virtual void OnWallCollision(const sCollisionResult& result);

	void PursueStart();
};

