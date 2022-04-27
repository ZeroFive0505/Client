#pragma once

#include "State.h"
#include "Cheerleader.h"

class CCheerleaderAttack :
	public CState
{
public:
	CCheerleaderAttack(CRCGEnemy* npc, CState* nextState);
	~CCheerleaderAttack();

public:
	virtual void Enter()
	{
		CCheerleader* cheerleader = (CCheerleader*)m_NPC;

		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && cheerleader->IsMovingStart() && !m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");

		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		CCheerleader* cheerleader = (CCheerleader*)m_NPC;

		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && cheerleader->IsMovingStart() && !m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		CState::Exit();
	}

private:
	enum CheerleaderAttackType
	{
		BACKFLIP = 0,
		CARTWHEEL,
		DIVEKICK,
		DOUBLEPOM,
		JAB,
		MAX
	};

	float m_AttackProb = 0.05f;

private:
	bool CannotAttackPlayer();
};

