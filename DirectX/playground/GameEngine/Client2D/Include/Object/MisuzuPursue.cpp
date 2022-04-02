#include "MisuzuPursue.h"
#include "Misuzu.h"
#include "MisuzuAttack.h"
#include "MisuzuBlock.h"
#include "Resource/ResourceManager.h"
#include "BossEventTrigger.h"
#include "RCGPilar.h"

CMisuzuPursue::CMisuzuPursue(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::PURSUE;
	m_ElapsedTime = 0.0f;
}

CMisuzuPursue::~CMisuzuPursue()
{
}

void CMisuzuPursue::Update()
{
	if (m_NPC->IsInvincible())
		return;

	if (m_NPC->OnGuard())
		return;

	m_ElapsedTime += CEngine::GetInst()->GetDeltaTime();

	CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	Vector3 playerPos = player->GetWorldPos();

	if (m_NPC->GetWorldPos().x < playerPos.x)
		m_NPC->SetFacingDirection(true);
	else
		m_NPC->SetFacingDirection(false);

	CMisuzu* misuzu = (CMisuzu*)m_NPC;

	if (m_ElapsedTime >= 1.5f)
	{
		EMisuzuPhase phase = misuzu->GetMisuzuPhase();

		switch (phase)
		{
		case EMisuzuPhase::PHASE1:
			m_NPC->GetNavAgent()->SetMoveSpeed(m_NPC->GetWalkSpeed());
			break;
		case EMisuzuPhase::PHASE2:
			m_NPC->GetNavAgent()->SetMoveSpeed(m_NPC->GetWalkSpeed() + 25.0f);
			break;
		case EMisuzuPhase::PHASE3:
			m_NPC->GetNavAgent()->SetMoveSpeed(m_NPC->GetWalkSpeed() + 50.0f);
			break;
		}

		m_NPC->GetSpriteComponent()->ChangeAnimation("Walk");

		CTileMapComponent* tileMap = CSceneManager::GetInst()->GetScene()->GetNavigationManager()->GetNavData();

		if (player->GetLeftWallCollideInfo())
		{
			if (player->GetBottomWallCollideInfo())
				m_NPC->Move(Vector3(playerPos.x + 50.0f, playerPos.y + 1.0f, 0.0f));
			else
				m_NPC->Move(Vector3(playerPos.x + 50.0f, playerPos.y, 0.0f));
		}
		else if (player->GetRightWallCollideInfo())
		{
			if (player->GetBottomWallCollideInfo())
				m_NPC->Move(Vector3(playerPos.x - 50.0f, playerPos.y + 1.0f, 0.0f));
			else
				m_NPC->Move(Vector3(playerPos.x - 50.0f, playerPos.y, 0.0f));
		}
		else
		{
			int randPos = rand() % 2;

			if (randPos == 0)
			{
				if (player->GetBottomWallCollideInfo())
					m_NPC->Move(Vector3(playerPos.x - 60.0f, playerPos.y + 1.0f, 0.0f));
				else
					m_NPC->Move(Vector3(playerPos.x - 60.0f, playerPos.y, 0.0f));
			}
			else
			{
				if (player->GetBottomWallCollideInfo())
					m_NPC->Move(Vector3(playerPos.x + 60.0f, playerPos.y + 1.0f, 0.0f));
				else
					m_NPC->Move(Vector3(playerPos.x + 60.0f, playerPos.y, 0.0f));
			}
		}

		m_ElapsedTime = 0.0f;
	}

	if (player->GetWorldPos().SqrDistance(m_NPC->GetWorldPos()) <= 3600.0f)
		m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");

	float prob;

	if (player->GetWorldPos().SqrDistance(m_NPC->GetWorldPos()) >= 640000.0f)
	{
		prob = (float)rand() / (float)RAND_MAX;

		if (prob <= 0.02f)
		{
			int choice = rand() % 2;

			if (choice == 0)
			{
				SAFE_DELETE(m_NextState);
				misuzu->Meteor();
				m_NPC->GetNavAgent()->SetInterrupted(true);
				m_NextState = (CState*)new CMisuzuAttack(m_NPC, m_NextState);
				CResourceManager::GetInst()->SoundPlay("Misuzu_meteor_drop_jump");
				m_Stage = EVENT::EXIT;
				return;
			}
			else
			{	
				SAFE_DELETE(m_NextState);
				misuzu->Tackle();
				m_NPC->GetNavAgent()->SetInterrupted(true);
				m_NextState = (CState*)new CMisuzuAttack(m_NPC, m_NextState);
				CResourceManager::GetInst()->SoundPlay("Misuzu_tackle_begin");
				m_Stage = EVENT::EXIT;
				return;
			}
		}
	}

	bool canAttack = CanAttackPlayer();

	if (canAttack && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
	{
		prob = (float)rand() / (float)RAND_MAX;

		if (prob < 0.5f && player->CheckState(EKyokoState::ATTACK))
		{
			SAFE_DELETE(m_NextState);
			m_NPC->GetNavAgent()->SetInterrupted(true);
			m_NextState = (CState*)new CMisuzuBlock(m_NPC, m_NextState);
			m_Stage = EVENT::EXIT;
			return;
		}

		prob = (float)rand() / (float)RAND_MAX;

		if (prob <= 0.2f)
		{
			SAFE_DELETE(m_NextState);
			m_NPC->GetNavAgent()->SetInterrupted(true);
			m_NextState = (CState*)new CMisuzuAttack(m_NPC, m_NextState);
			m_Stage = EVENT::EXIT;
			return;
		}
	}

	if (!canAttack &&
		player->GetWorldPos().Distance(m_NPC->GetWorldPos()) >= 200.0f &&
		player->GetWorldPos().Distance(m_NPC->GetWorldPos()) <= 300.0f &&
		fabsf(player->GetWorldPos().y - m_NPC->GetWorldPos().y) <= 5.0f)
	{
		if (player->CheckState(EKyokoState::DOWN) || player->CheckState(EKyokoState::KNOCKDOWN) ||
			player->CheckState(EKyokoState::BLOWNBACK) || player->CheckState(EKyokoState::INVINCIBLE))
			return;

		if (player->IsInvincible() || player->IsPhysicsOn())
			return;

		m_NPC->GetNavAgent()->SetInterrupted(true);
		misuzu->WUPunch();
	}
}

bool CMisuzuPursue::CanAttackPlayer()
{
	CColliderBox2D* body = m_NPC->GetBodyCollider();
	CColliderBox2D* bottom = m_NPC->GetBottomCollider();

	std::unordered_map<CGameObject*, int> collisions;

	{
		auto iter = body->GetPrevColliderList().cbegin();
		auto iterEnd = body->GetPrevColliderList().cend();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetCollisionProfile()->name == "Player")
			{
				collisions[(*iter)->GetGameObject()]++;
			}
		}
	}

	{
		auto iter = bottom->GetPrevColliderList().cbegin();
		auto iterEnd = bottom->GetPrevColliderList().cend();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetCollisionProfile()->name == "PlayerBottom")
			{
				collisions[(*iter)->GetGameObject()]++;
			}
		}
	}

	CGameObject* obj = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	if (collisions[obj] >= 2)
	{
		CRCGPlayer* player = (CRCGPlayer*)obj;

		if (player->CheckState(EKyokoState::DOWN) || player->CheckState(EKyokoState::KNOCKDOWN) ||
			player->CheckState(EKyokoState::BLOWNBACK) || player->CheckState(EKyokoState::INVINCIBLE))
			return false;

		return true;
	}
	else
		return false;
}
