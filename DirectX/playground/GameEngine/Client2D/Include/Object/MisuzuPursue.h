#pragma once

#include "State.h"

class CMisuzuPursue :
	public CState
{
public:
	CMisuzuPursue(CRCGEnemy* npc, CState* nextState);
	~CMisuzuPursue();

private:
	float m_ElapsedTime;

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

public:
	bool CanAttackPlayer();
};

