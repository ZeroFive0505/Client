#pragma once

#include "RCGEnemy.h"
#include "State.h"

enum class ECheerleaderMoveSet
{
	IDLE,
	BACKFLIP,
	CARTWHEEL,
	DIVEKICK,
	DOUBLEPOM,
	JAB
};


class CCheerleader :
	public CRCGEnemy
{
	friend class CScene;

protected:
	CCheerleader();
	CCheerleader(const CCheerleader& obj);
	virtual ~CCheerleader();

private:
	Vector3 m_Offset;
	std::unordered_map<ECheerleaderMoveSet, sEnemyMove> m_mapMoveset;
	ECheerleaderMoveSet m_CurrentMove;
	CState* m_State;
	CState* m_NextState;
	bool m_CartWheelPush;
	int m_CartWheelDirection;
	bool m_MoveStart;
	bool m_DiveKickStart;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CCheerleader* Clone();

private:
	void SetMoveSetInfo();

public:
	inline void BackFlip()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Backflip");
		m_CurrentMove = ECheerleaderMoveSet::BACKFLIP;
	}

	inline void CartWheel()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Cartwheel");
		m_Invincible = true;
		m_Push = true;
		m_CartWheelPush = true;

		m_CartWheelDirection = rand() % 2;

		m_CurrentMove = ECheerleaderMoveSet::CARTWHEEL;
	}

	inline void DiveKick()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		JumpStart();
		PushState(EEnemyState::ATTACK);
		m_CurrentMove = ECheerleaderMoveSet::DIVEKICK;
	}

	inline void DoublePom()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Doublepom");
		m_CurrentMove = ECheerleaderMoveSet::DOUBLEPOM;
	}

	inline void Jab()
	{
		if (CheckState(EEnemyState::ATTACK) || CheckState(EEnemyState::GETHIT))
			return;

		PushState(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Jab");
		m_CurrentMove = ECheerleaderMoveSet::JAB;
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
		m_CurrentMove = ECheerleaderMoveSet::IDLE;
		DeactivateHitBox();
	}

	inline void CartWheelEnd()
	{
		m_Invincible = false;
		m_Push = false;
		m_CartWheelPush = false;
		PopStateEnd(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Idle");
		m_CurrentMove = ECheerleaderMoveSet::IDLE;
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

public:
	inline bool IsMovingStart() const
	{
		return m_MoveStart;
	}

	inline void GravityOff()
	{
		m_GravityEnable = false;
	}

	inline void DiveKickBegin()
	{
		m_Sprite->ChangeAnimation("Divekick_middle");
	}

	inline void DiveKickFall()
	{
		ActivateHitBox();
		m_GravityEnable = true;
		m_Gravity *= 4.0f;
		m_Push = true;
	}

	inline void DiveKickEnd()
	{
		PopStateEnd(EEnemyState::ATTACK);
		m_Sprite->ChangeAnimation("Idle");
		m_CurrentMove = ECheerleaderMoveSet::IDLE;
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
	virtual void OnGround(const sCollisionResult& result);
	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right);

public:
	void HitBoxUpdate();
	void HitBoxCheck(const sCollisionResult& result);
	
public:
	void SetIdleState();
	void SetPursueState();

};

