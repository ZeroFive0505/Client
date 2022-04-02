#include "SchoolBoyIdle.h"

CSchoolBoyIdle::CSchoolBoyIdle(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::IDLE;
	m_DetectDist = 300.0f;
}

CSchoolBoyIdle::~CSchoolBoyIdle()
{
}

void CSchoolBoyIdle::Update()
{
	if (PlayerDetected())
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)(new CSchoolBoyPursue(m_NPC, m_NextState));
		m_Stage = EVENT::EXIT;
		return;
	}
}
