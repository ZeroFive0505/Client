#pragma once

#include "RCGEnemy.h"
#include "State.h"

enum class ESchoolGirlMoveSet
{
	IDLE,
	CRESENTKICK,
	JAB,
	JUMPKNEE,
	KICK,
	TORNATOKICK
};

class CSchoolGirl :
	public CRCGEnemy
{
	friend class CScene;
protected:
	CSchoolGirl();
	CSchoolGirl(const CSchoolGirl& obj);
	virtual ~CSchoolGirl();

private:
	std::unordered_map<ESchoolGirlMoveSet, sEnemyMove> m_mapMoveset;
	ESchoolGirlMoveSet m_CurrentMove;
	CState* m_State;
	CState* m_NextState;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSchoolGirl* Clone();

private:
	void SetMoveSetInfo();

public:
	inline void CresentKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Cresentkick");
		m_CurrentMove = ESchoolGirlMoveSet::CRESENTKICK;
	}

	inline void Jab()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Jab");
		m_CurrentMove = ESchoolGirlMoveSet::JAB;
	}

	inline void JumpKnee()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Jumpknee");
		m_CurrentMove = ESchoolGirlMoveSet::JUMPKNEE;
	}

	inline void Kick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Kick");
		m_CurrentMove = ESchoolGirlMoveSet::KICK;
	}

	inline void TornadoKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Tornadokick_start");
		m_CurrentMove = ESchoolGirlMoveSet::TORNATOKICK;
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
		m_CurrentMove = ESchoolGirlMoveSet::IDLE;
		DeactivateHitBox();
	}

	inline void AirAttackEndCallback()
	{
		m_Sprite->ChangeAnimation("Jump");
		m_CurrentMove = ESchoolGirlMoveSet::IDLE;
	}

	inline void TornadoKickStart()
	{
		JumpStart();
		m_Sprite->ChangeAnimation("Tornadokick_loop");
		m_Push = true;
	}
	
	inline void TornadoKickEnd()
	{
		m_CurrentMove = ESchoolGirlMoveSet::IDLE;
		DeactivateHitBox();
	}

public:
	void HitBoxUpdate();
	void HitBoxCheck(const sCollisionResult& result);
	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right);

public:
	void SetIdleState();
	

	void SetPursueState();

};

