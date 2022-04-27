#pragma once

#include "State.h"
#include "MT.h"

class CMTPursue :
	public CState
{
public:
	CMTPursue(CRCGEnemy* npc, CState* nextState);
	~CMTPursue();

private:
	float m_RunDist;
	float m_ElapsedTime;

public:
	virtual void Enter()
	{
		CMT* mt = (CMT*)m_NPC;

		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && mt->IsMovingStart() && !m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		else if (m_NPC->IsSpawning())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");


		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		CMT* mt = (CMT*)m_NPC;

		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && mt->IsMovingStart() && !m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");

		CState::Exit();
	}

private:
	bool CanAttackPlayer();
};

