#pragma once

#include "State.h"

class CSchoolBoyFallback :
	public CState
{
public:
	CSchoolBoyFallback(CRCGEnemy* npc, CState* nextState);
	virtual ~CSchoolBoyFallback();

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
	float m_BackToPursueProb = 0.01f;
};

