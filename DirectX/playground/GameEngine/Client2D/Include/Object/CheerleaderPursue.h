#pragma once

#include "State.h"
#include "Cheerleader.h"

class CCheerleaderPursue :
	public CState
{
public:
	CCheerleaderPursue(CRCGEnemy* npc, CState* nextState);
	~CCheerleaderPursue();

private:
	float m_RunDist;
	float m_ElapsedTime;

public:
	virtual void Enter()
	{
		CCheerleader* cheerleader = (CCheerleader*)m_NPC;

		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && cheerleader->IsMovingStart())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		else if(m_NPC->IsSpawning())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");


		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		CState::Exit();
	}

private:
	bool CanAttackPlayer();
};

