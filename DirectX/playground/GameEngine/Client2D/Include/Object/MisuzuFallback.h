#pragma once

#include "State.h"

class CMisuzuFallback :
	public CState
{
public:
	CMisuzuFallback(CRCGEnemy* npc, CState* nextState);
	~CMisuzuFallback();

public:
	virtual void Enter()
	{
		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && !m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		CState::Exit();
	}
};

