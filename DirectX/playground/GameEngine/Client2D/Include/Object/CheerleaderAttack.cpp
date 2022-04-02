#include "CheerleaderAttack.h"
#include "CheerleaderFallback.h"
#include "CheerleaderPursue.h"
#include "Cheerleader.h"

CCheerleaderAttack::CCheerleaderAttack(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::ATTACK;
}

CCheerleaderAttack::~CCheerleaderAttack()
{
}

void CCheerleaderAttack::Update()
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
		m_NextState = (CState*)(new CCheerleaderFallback(m_NPC, m_NextState));
		m_Stage = EVENT::EXIT;
		return;
	}

	CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	bool cannotAttack = CannotAttackPlayer();

	if (cannotAttack)
	{
		prob = (float)rand() / (float)RAND_MAX;

		if (prob <= 0.2f && m_NPC->GetCurrentState() == (int)EEnemyState::NORMAL)
		{
			SAFE_DELETE(m_NextState);
			m_NextState = (CState*)(new CCheerleaderPursue(m_NPC, m_NextState));
			m_Stage = EVENT::EXIT;
			return;
		}
	}

	CCheerleader* cheerleader = (CCheerleader*)m_NPC;


	if (!cannotAttack)
	{
		if (m_NPC->IsOnGround())
		{
			int branch = rand() % (int)CheerleaderAttackType::MAX;

			switch ((CheerleaderAttackType)branch)
			{
			case CheerleaderAttackType::JAB:
				cheerleader->Jab();
				break;
			case CheerleaderAttackType::BACKFLIP:
				cheerleader->BackFlip();
				break;
			case CheerleaderAttackType::DOUBLEPOM:
				cheerleader->DoublePom();
				break;
			case CheerleaderAttackType::CARTWHEEL:
				cheerleader->CartWheel();
				break;
			}
		}
	}
}

bool CCheerleaderAttack::CannotAttackPlayer()
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
