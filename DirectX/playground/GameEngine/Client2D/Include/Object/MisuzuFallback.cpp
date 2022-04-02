#include "MisuzuFallback.h"
#include "MisuzuPursue.h"
#include "MisuzuAttack.h"
#include "Misuzu.h"

CMisuzuFallback::CMisuzuFallback(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::FALLBACK;
}

CMisuzuFallback::~CMisuzuFallback()
{
}

void CMisuzuFallback::Update()
{
	if (m_NPC->IsInvincible())
		return;
	
	if (m_NPC->OnGuard())
		return;

	float prob = (float)rand() / (float)RAND_MAX;

	if (prob <= 0.01f)
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)new CMisuzuPursue(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}

	CMisuzu* misuzu = (CMisuzu*)m_NPC;

	CGameObject* obj = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	if (m_NPC->GetWorldPos().x < obj->GetWorldPos().x)
	{
		if (!m_NPC->GetLeftWallCollideInfo())
		{
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
