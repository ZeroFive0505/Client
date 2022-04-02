#pragma once

#include "State.h"

class CCheerleaderBlock :
	public CState
{
public:
	CCheerleaderBlock(CRCGEnemy* npc, CState* nextState);
	~CCheerleaderBlock();

public:
	virtual void Enter()
	{
		m_NPC->GetSpriteComponent()->ChangeAnimation("Block");
		m_NPC->GuardOn();
		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		m_NPC->GetSpriteComponent()->ChangeAnimation("Block_end");
		CState::Exit();
	}

private:
	bool BlockEndCondition();
};

