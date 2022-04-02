#pragma once

#include "State.h"
#include "Engine.h"
#include "SchoolBoyAttack.h"

class CSchoolBoyPursue :
	public CState
{
public:
	CSchoolBoyPursue(CRCGEnemy* npc, CState* nextState);
	virtual ~CSchoolBoyPursue();

private:
	float m_RunDist;
	float m_ElapsedTime;

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
	bool CanAttackPlayer();
};

