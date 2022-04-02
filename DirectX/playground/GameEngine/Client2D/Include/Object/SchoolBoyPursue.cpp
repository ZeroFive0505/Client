#include "SchoolBoyPursue.h"
#include "SchoolBoyBlock.h"

CSchoolBoyPursue::CSchoolBoyPursue(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::PURSUE;
	m_ElapsedTime = 0.0f;
	m_RunDist = 500.0f;
}

CSchoolBoyPursue::~CSchoolBoyPursue()
{
}

void CSchoolBoyPursue::Update()
{
	if (m_NPC->IsInvincible())
		return;

	if (m_NPC->OnGuard())
		return;

	m_ElapsedTime += 1.0f / 60.0f;

	CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	Vector3 playerPos = player->GetWorldPos();


	if (m_NPC->GetWorldPos().x < playerPos.x)
		m_NPC->SetFacingDirection(true);
	else
		m_NPC->SetFacingDirection(false);

	if (m_NPC->GetCurrentState() != (int)EEnemyState::NORMAL)
		return;

	if (m_ElapsedTime >= 1.5f)
	{
		m_NPC->GetNavAgent()->SetMoveSpeed(m_NPC->GetWalkSpeed());

		if (m_NPC->IsOnGround())
			m_NPC->GetSpriteComponent()->ChangeAnimation("Walk");

		if (player->GetWorldPos().SqrDistance(m_NPC->GetWorldPos()) >= 640000.0f)
		{
			m_NPC->GetNavAgent()->SetMoveSpeed(m_NPC->GetRunSpeed());

			if (m_NPC->IsOnGround())
				m_NPC->GetSpriteComponent()->ChangeAnimation("Run");
		}

		CTileMapComponent* tileMap = CSceneManager::GetInst()->GetScene()->GetNavigationManager()->GetNavData();

		if (player->GetLeftWallCollideInfo())
		{
			if (player->GetBottomWallCollideInfo())
				m_NPC->Move(Vector3(playerPos.x + 40.0f, playerPos.y + 1.0f, 0.0f));
			else
				m_NPC->Move(Vector3(playerPos.x + 40.0f, playerPos.y, 0.0f));
		}
		else if (player->GetRightWallCollideInfo())
		{
			if (player->GetBottomWallCollideInfo())
				m_NPC->Move(Vector3(playerPos.x - 40.0f, playerPos.y + 1.0f, 0.0f));
			else
				m_NPC->Move(Vector3(playerPos.x - 40.0f, playerPos.y, 0.0f));
		}
		else
		{
			int randPos = rand() % 2;

			if (randPos == 0)
			{
				if (player->GetBottomWallCollideInfo())
					m_NPC->Move(Vector3(playerPos.x - 40.0f, playerPos.y + 1.0f, 0.0f));
				else
					m_NPC->Move(Vector3(playerPos.x - 40.0f, playerPos.y, 0.0f));
			}
			else
			{
				if (player->GetBottomWallCollideInfo())
					m_NPC->Move(Vector3(playerPos.x + 40.0f, playerPos.y + 1.0f, 0.0f));
				else
					m_NPC->Move(Vector3(playerPos.x + 40.0f, playerPos.y, 0.0f));
			}
		}


		m_ElapsedTime = 0.0f;
	}

	if (player->GetWorldPos().SqrDistance(m_NPC->GetWorldPos()) <= 50.0f)
		m_NPC->GetSpriteComponent()->ChangeAnimation("Idle");

	if (CanAttackPlayer() && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
	{
		float prob = (float)rand() / (float)RAND_MAX;

		if (prob < 0.5f && player->CheckState(EKyokoState::ATTACK))
		{
			SAFE_DELETE(m_NextState);
			m_NPC->GetNavAgent()->SetInterrupted(true);
			m_NextState = (CState*)(new CSchoolBoyBlock(m_NPC, m_NextState));
			m_Stage = EVENT::EXIT;
			return;
		}

		prob = (float)rand() / (float)RAND_MAX;

		if (prob <= 0.2f)
		{
			SAFE_DELETE(m_NextState);
			m_NPC->GetNavAgent()->SetInterrupted(true);
			m_NextState = (CState*)(new CSchoolBoyAttack(m_NPC, m_NextState));
			m_Stage = EVENT::EXIT;
			return;
		}
	}
}

bool CSchoolBoyPursue::CanAttackPlayer()
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
