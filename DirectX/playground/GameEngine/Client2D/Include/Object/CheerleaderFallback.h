#pragma once

#include "State.h"
#include "Cheerleader.h"

class CCheerleaderFallback :
	public CState
{
public:
	CCheerleaderFallback(CRCGEnemy* npc, CState* nextState);
	~CCheerleaderFallback();

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
	float m_BackToPursueProb = 0.01f;
	bool m_DiveKick;
};

