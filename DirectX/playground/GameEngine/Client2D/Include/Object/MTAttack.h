#pragma once

#include "State.h"
#include "MT.h"

class CMTAttack :
	public CState
{
public:
	CMTAttack(CRCGEnemy* npc, CState* nextState);
	~CMTAttack();

public:
	virtual void Enter()
	{
		CMT* mt = (CMT*)m_NPC;

		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && mt->IsMovingStart())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");


		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		CState::Exit();
	}

private:
	enum MTAttackType
	{
		CROSSPUNCH = 0,
		DODGEKICK,
		JUMPKICK,
		JUMPBACKKICK,
		SNAPKICK,
		MAX
	};

	float m_AttackProb = 0.05f;

private:
	bool CannotAttackPlayer();
};

