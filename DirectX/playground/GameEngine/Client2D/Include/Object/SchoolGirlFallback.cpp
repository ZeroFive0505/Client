#include "SchoolGirlFallback.h"
#include "SchoolGirlPursue.h"

CSchoolGirlFallback::CSchoolGirlFallback(CRCGEnemy* npc, CState* nextState) : 
	CState(npc, nextState)
{
	m_State = STATE::FALLBACK;
}

CSchoolGirlFallback::~CSchoolGirlFallback()
{
}

void CSchoolGirlFallback::Update()
{
	if (m_NPC->IsInvincible())
		return;

	if (m_NPC->OnGuard())
		return;

	float prob = (float)rand() / (float)RAND_MAX;

	if (prob <= m_BackToPursueProb && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)new CSchoolGirlPursue(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}

	CGameObject* obj = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	if (m_NPC->GetCurrentState() != (int)EEnemyState::NORMAL)
		return;

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
