#include "CheerleaderIdle.h"
#include "Cheerleader.h"
#include "CheerleaderPursue.h"

CCheerleaderIdle::CCheerleaderIdle(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::IDLE;
	m_DetectDist = 300.0f;
}

CCheerleaderIdle::~CCheerleaderIdle()
{
}

void CCheerleaderIdle::Update()
{
	if (PlayerDetected())
	{
		CCheerleader* cheerleader = (CCheerleader*)m_NPC;
		cheerleader->AnimStart();
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)new CCheerleaderPursue(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}

	m_NPC->GetSpriteComponent()->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(0);
}
