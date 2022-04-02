#pragma once

#include "State.h"
#include "Cheerleader.h"

class CCheerleaderIdle :
	public CState
{
public:
	CCheerleaderIdle(CRCGEnemy* npc, CState* nextState);
	~CCheerleaderIdle();

public:
	virtual void Enter()
	{
		m_NPC->GetSpriteComponent()->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(0);
		CState::Enter();
	}

	virtual void Update();

	virtual void Exit()
	{
		m_NPC->PlayerDetect();
		CState::Exit();
	}

private:
	float m_DetectDist;

private:
	inline bool PlayerDetected()
	{
		CGameObject* player = CSceneManager::GetInst()->GetScene()->GetPlayerObject();
		return m_NPC->GetWorldPos().SqrDistance(player->GetWorldPos()) <= (m_DetectDist * m_DetectDist);
	}
};

