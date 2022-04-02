#pragma once

#include "State.h"
#include "SchoolBoyPursue.h"

class CSchoolBoyIdle :
	public CState
{
public:
	CSchoolBoyIdle(CRCGEnemy* npc, CState* nextState);
	virtual ~CSchoolBoyIdle();
private:
	float m_DetectDist;
public:
	virtual void Enter()
	{
		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		m_NPC->PlayerDetect();
		CState::Exit();
	}

private:
	inline bool PlayerDetected()
	{
		CGameObject* player = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

		return m_NPC->GetWorldPos().SqrDistance(player->GetWorldPos()) <= (m_DetectDist * m_DetectDist);
	}
};

