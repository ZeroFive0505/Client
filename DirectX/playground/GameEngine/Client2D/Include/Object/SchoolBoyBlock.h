#pragma once

#include "State.h"

class CSchoolBoyBlock :
	public CState
{
public:
	CSchoolBoyBlock(CRCGEnemy* npc, CState* nextState);
	~CSchoolBoyBlock();

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

