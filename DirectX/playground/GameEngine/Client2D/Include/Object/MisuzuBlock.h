#pragma once

#include "State.h"

class CMisuzuBlock :
	public CState
{
public:
	CMisuzuBlock(CRCGEnemy* npc, CState* nextState);
	~CMisuzuBlock();

public:
	virtual void Enter();

	virtual void Update();

	virtual void Exit()
	{
		m_NPC->GetSpriteComponent()->ChangeAnimation("Block_end");

		CState::Exit();
	}

private:
	bool BlockEndCondition();
};

