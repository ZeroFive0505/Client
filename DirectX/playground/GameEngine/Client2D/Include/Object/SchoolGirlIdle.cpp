#include "SchoolGirlIdle.h"
#include "SchoolGirlPursue.h"

CSchoolGirlIdle::CSchoolGirlIdle(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::IDLE;
	m_DetectDist = 300.0f;
}

CSchoolGirlIdle::~CSchoolGirlIdle()
{
}

void CSchoolGirlIdle::Update()
{
	if (PlayerDetected())
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)new CSchoolGirlPursue(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}
}
