#pragma once

#include "State.h"
#include "Engine.h"

class CSchoolBoyAttack :
	public CState
{
public:
	CSchoolBoyAttack(CRCGEnemy* npc, CState* nextState);
	virtual ~CSchoolBoyAttack();

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
	enum SchoolBoyAttackType
	{
		AXEKICK = 0,
		JUMPPUNCH,
		SANDTOSS,
		SIDEKICK,
		PUNCH,
		ELBOWUPPER,
		MAX
	};

	float m_AttackProb = 0.05f;


private:
	bool CannotAttackPlayer();
};

