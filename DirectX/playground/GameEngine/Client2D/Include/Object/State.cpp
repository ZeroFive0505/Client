#include "State.h"

CState::CState(CRCGEnemy* npc, CState* nextState)
{
	m_NPC = npc;
	m_NextState = nextState;
}

CState::~CState()
{
	m_NPC = nullptr;
}
