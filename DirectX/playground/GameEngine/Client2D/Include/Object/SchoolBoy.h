#pragma once

#include "RCGEnemy.h"
#include "State.h"

enum class ESchoolBoyMoveSet
{
	IDLE,
	AXEKICK,
	SIDEKICK,
	ELBOW_UPPERCUT,
	JUMP_PUNCH,
	RIGHTSTRAIGHT_PUNCH,
	SANDTOSS
};

class CSchoolBoy :
	public CRCGEnemy
{
	friend class CScene;

protected:
	CSchoolBoy();
	CSchoolBoy(const CSchoolBoy& obj);
	virtual ~CSchoolBoy();

private:
	Vector3 m_Offset;
	std::unordered_map<ESchoolBoyMoveSet, sEnemyMove> m_mapMoveset;
	ESchoolBoyMoveSet m_CurrentMove;
	CState* m_State;
	CState* m_NextState;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSchoolBoy* Clone();

private:
	void SetMoveSetInfo();

public:
	inline void Axekick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Axekick");
		m_CurrentMove = ESchoolBoyMoveSet::AXEKICK;
	}

	inline void Sidekick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Sidekick");
		m_CurrentMove = ESchoolBoyMoveSet::SIDEKICK;
	}

	inline void ElbowUpper()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Elbowupper");
		m_CurrentMove = ESchoolBoyMoveSet::ELBOW_UPPERCUT;
	}

	inline void JumpPunch()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Jumppunch");
		m_CurrentMove = ESchoolBoyMoveSet::JUMP_PUNCH;
	}

	inline void RightStraight()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Rightstraight");
		m_CurrentMove = ESchoolBoyMoveSet::RIGHTSTRAIGHT_PUNCH;
	}

	inline void SandToss()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Sandtoss");
		m_CurrentMove = ESchoolBoyMoveSet::SANDTOSS;
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
		m_CurrentMove = ESchoolBoyMoveSet::IDLE;
		DeactivateHitBox();
	}

	inline void JumpPunchEndCallback()
	{
		m_Sprite->ChangeAnimation("Jump");
		m_CurrentMove = ESchoolBoyMoveSet::IDLE;
	}

public:
	void HitBoxUpdate();
	void HitBoxCheck(const sCollisionResult& result);

public:
	void SetIdleState();
	void SetPursueState();
};

