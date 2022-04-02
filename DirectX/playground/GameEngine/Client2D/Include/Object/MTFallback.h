#pragma once

#include "State.h"
#include "MT.h"

class CMTFallback :
	public CState
{
public:
	CMTFallback(CRCGEnemy* npc, CState* nextState);
	~CMTFallback();

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
	float m_BackToPursueProb = 0.01f;
};

