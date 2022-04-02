#include "MTIdle.h"
#include "MTPursue.h"
#include "MT.h"

CMTIdle::CMTIdle(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::IDLE;
	m_DetectDist = 300.0f;
}

CMTIdle::~CMTIdle()
{
}

void CMTIdle::Update()
{
	if (PlayerDetected())
	{
		CMT* mt = (CMT*)m_NPC;
		mt->AnimStart();
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)new CMTPursue(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}

	m_NPC->GetSpriteComponent()->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(0);
}
