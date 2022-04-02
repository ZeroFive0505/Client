#pragma once

#include "State.h"

class CSchoolGirlPursue :
	public CState
{
public:
	CSchoolGirlPursue(CRCGEnemy* npc, CState* nextState);
	~CSchoolGirlPursue();


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

