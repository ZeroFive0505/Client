#pragma once

#include "State.h"

class CSchoolGirlFallback :
	public CState
{
public:
	CSchoolGirlFallback(CRCGEnemy* npc, CState* nextState);
	~CSchoolGirlFallback();

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

