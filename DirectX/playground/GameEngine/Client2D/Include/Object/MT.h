#pragma once

#include "RCGEnemy.h"
#include "State.h"

enum class EMTMoveSet
{
	IDLE,
	CROSSPUNCH,
	DODGEKICK,
	JUMPBACKKICK,
	SNAPKICK,
	JUMPKICK,
};

class CMT :
	public CRCGEnemy
{
	friend class CScene;
protected:
	CMT();
	CMT(const CMT& obj);
	virtual ~CMT();

private:
	Vector3 m_Offset;
	std::unordered_map<EMTMoveSet, sEnemyMove> m_mapMoveset;
	EMTMoveSet m_CurrentMove;
	CState* m_State;
	CState* m_NextState;
	bool m_MoveStart;
	bool m_Dodge;
	bool m_Kick;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMT* Clone();

private:
	void SetMoveSetInfo();

public:
	inline void CrossPunch()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Crosspunch");
		m_CurrentMove = EMTMoveSet::CROSSPUNCH;
	}

	inline void DodgeKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Dodgekick");
		m_CurrentMove = EMTMoveSet::DODGEKICK;
	}

	inline void JumpBackKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Jumpbackkick");
		m_CurrentMove = EMTMoveSet::JUMPBACKKICK;
	}

	inline void SnapKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Snapkick");
		m_CurrentMove = EMTMoveSet::SNAPKICK;
	}

	inline void JumpKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Jumpkick");
		m_CurrentMove = EMTMoveSet::JUMPKICK;
	}

	inline void DodgeStart()
	{
		m_Push = true;
		m_Dodge = true;
		m_Kick = false;
	}

	inline void DodgeKickStart()
	{
		m_Dodge = false;
		m_Kick = true;
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
		m_CurrentMove = EMTMoveSet::IDLE;
		m_Push = false;
		DeactivateHitBox();
	}

	inline void JumpKickEndCallback()
	{
		m_Sprite->ChangeAnimation("Jump");
		m_CurrentMove = EMTMoveSet::IDLE;
	}
	
	inline void AnimStart()
	{
		m_Sprite->GetAnimationInstance()->Play();
	}

	inline void AlertFinish()
	{
		m_MoveStart = true;
		m_Sprite->ChangeAnimation("Idle");
	}

	inline bool IsMovingStart() const
	{
		return m_MoveStart;
	}

public:
	inline void SpriteOffset()
	{
		m_Offset = Vector3(0.0f, 35.0f, 0.0f);
		Vector3 offset = m_Sprite->GetWorldPos() + m_Offset;
		m_Sprite->SetWorldPos(offset);
	}

	inline void ResetOffset()
	{
		m_Sprite->SetWorldPos(m_Sprite->GetWorldPos() - m_Offset);
		m_Offset = Vector3(0.0f, 0.0f, 0.0f);
	}

public:
	void HitBoxUpdate();
	void HitBoxCheck(const sCollisionResult& result);
	virtual void OnGround(const sCollisionResult& result);
	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right);
	
public:
	void SetIdleState();

	void SetPursueState();

};

