#include "CheerleaderFallback.h"
#include "CheerleaderPursue.h"
#include "Engine.h"

CCheerleaderFallback::CCheerleaderFallback(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::FALLBACK;
	m_DiveKick = true;
}

CCheerleaderFallback::~CCheerleaderFallback()
{
}

void CCheerleaderFallback::Update()
{
	if (m_NPC->IsInvincible())
		return;

	if (m_NPC->OnGuard())
		return;

	CGameObject* obj = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	float prob = (float)rand() / (float)RAND_MAX;

	prob = (float)rand() / (float)RAND_MAX;

	if (prob <= 0.2f && m_DiveKick)
	{
		CCheerleader* cheerleader = (CCheerleader*)m_NPC;
		cheerleader->DiveKick();
		m_DiveKick = false;
		return;
	}

	prob = (float)rand() / (float)RAND_MAX;

	if (prob <= m_BackToPursueProb && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
	{
		SAFE_DELETE(m_NextState);
		m_NextState = new CCheerleaderPursue(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}

	if (m_NPC->GetWorldPos().x < obj->GetWorldPos().x)
	{
		if (!m_NPC->GetLeftWallCollideInfo())
		{
			if (m_NPC->IsOnGround())
				m_NPC->GetSpriteComponent()->ChangeAnimation("Walk");
			m_NPC->MoveLeft();
		}
		else
		{
			if (m_NPC->IsOnGround())
				m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		}
	}
	else
	{
		if (!m_NPC->GetRightWallCollideInfo())
		{
			if (m_NPC->IsOnGround())
				m_NPC->GetSpriteComponent()->ChangeAnimation("Walk");
			m_NPC->MoveRight();
		}
		else
		{
			if (m_NPC->IsOnGround())
				m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");
		}
	}
}
