#include "MTAttack.h"
#include "MTFallback.h"
#include "MTPursue.h"
#include "MT.h"

CMTAttack::CMTAttack(CRCGEnemy* npc, CState* nextState) :
    CState(npc, nextState)
{
    m_State = STATE::ATTACK;
}

CMTAttack::~CMTAttack()
{
}

void CMTAttack::Update()
{
	if (m_NPC->IsInvincible())
		return;

	if (m_NPC->OnGuard())
		return;

	float prob = (float)rand() / (float)RAND_MAX;

	if (prob > m_AttackProb)
		return;

	prob = (float)rand() / (float)RAND_MAX;

	if (prob <= 0.05f && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)(new CMTFallback(m_NPC, m_NextState));
		m_Stage = EVENT::EXIT;
		return;
	}

	CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	if (player->IsJumping())
		m_NPC->JumpStart();

	bool cannotAttack = CannotAttackPlayer();

	if (cannotAttack)
	{
		prob = (float)rand() / (float)RAND_MAX;

		if (prob <= 0.2f && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
		{
			SAFE_DELETE(m_NextState);
			m_NextState = (CState*)(new CMTPursue(m_NPC, m_NextState));
			m_Stage = EVENT::EXIT;
			return;
		}
	}

	CMT* mt = (CMT*)m_NPC;


	if (!cannotAttack)
	{
		if (m_NPC->IsJumping())
			mt->JumpKick();
		else if (m_NPC->IsOnGround())
		{
			int branch = rand() % (int)MTAttackType::MAX;

			switch ((MTAttackType)branch)
			{
			case MTAttackType::CROSSPUNCH:
				mt->CrossPunch();
				break;
			case MTAttackType::DODGEKICK:
				mt->DodgeKick();
				break;
			case MTAttackType::JUMPBACKKICK:
				mt->JumpBackKick();
				break;
			case MTAttackType::SNAPKICK:
				mt->SnapKick();
				break;
			}
		}
	}
}

bool CMTAttack::CannotAttackPlayer()
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

	if (collisions[obj] < 2)
		return true;
	else
	{
		CRCGPlayer* player = (CRCGPlayer*)obj;

		if (player->CheckState(EKyokoState::DOWN) || player->CheckState(EKyokoState::KNOCKDOWN) ||
			player->CheckState(EKyokoState::BLOWNBACK) || player->CheckState(EKyokoState::INVINCIBLE))
			return true;

		if (player->IsInvincible() || player->IsPhysicsOn())
			return true;

		return false;
	}
}
