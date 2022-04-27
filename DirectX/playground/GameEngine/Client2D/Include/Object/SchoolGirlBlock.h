#pragma once

#include "State.h"
#include "SchoolGirlPursue.h"

class CSchoolGirlBlock :
	public CState
{
public:
	CSchoolGirlBlock(CRCGEnemy* npc, CState* nextState);
	~CSchoolGirlBlock();

public:
	virtual void Enter()
	{
		if (m_NPC->IsInvincible())
		{
			SAFE_DELETE(m_NextState);
			m_NextState = (CState*)(new CSchoolGirlPursue(m_NPC, m_NextState));
			m_Stage = EVENT::EXIT;
			return;
		}

		m_NPC->GetSpriteComponent()->ChangeAnimation("Block");
		m_NPC->GuardOn();
		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		if(!m_NPC->IsInvincible())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Block_end");
		CState::Exit();
	}

private:
	bool BlockEndCondition();
};

