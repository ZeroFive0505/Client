#pragma once

#include "State.h"

class CSchoolGirlAttack :
	public CState
{
public:
	CSchoolGirlAttack(CRCGEnemy* npc, CState* nextState);
	~CSchoolGirlAttack();

public:
	virtual void Enter()
	{
		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		CState::Exit();
	}

private:
	enum SchoolGirlAttackType
	{
		CRESENTKICK = 0,
		JAB,
		JUMPKNEE,
		KICK,
		TORNADOKICK,
		MAX
	};

	float m_AttackProb = 0.05f;

private:
	bool CannotAttackPlayer();
};

