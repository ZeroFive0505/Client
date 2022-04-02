#pragma once

#include "State.h"

class CMisuzuAttack :
	public CState
{
public:
	CMisuzuAttack(CRCGEnemy* npc, CState* nextState);
	~CMisuzuAttack();

public:
	virtual void Enter()
	{
		if (m_NPC->IsOnGround() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL && !m_NPC->OnGuard() && !m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		CState::Enter();
		m_LerpTime = 0.0f;
	}

	virtual void Update();

	virtual void Exit()
	{
		CState::Exit();
	}

private:
	enum EMisuzuAttackType
	{
		ELBOW = 0,
		WUPUNCH,
		SLAP,
		MAX
	};

	float m_LerpTime;

private:
	bool CannotAttackPlayer();
	void Tackle();
	void Meteor();
};

