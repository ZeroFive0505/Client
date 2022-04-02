#include "SchoolGirlAttack.h"
#include "SchoolGirlFallback.h"
#include "SchoolGirlPursue.h"
#include "SchoolGirl.h"

CSchoolGirlAttack::CSchoolGirlAttack(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::ATTACK;
}

CSchoolGirlAttack::~CSchoolGirlAttack()
{
}

void CSchoolGirlAttack::Update()
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
		m_NextState = (CState*)(new CSchoolGirlFallback(m_NPC, m_NextState));
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
			m_NextState = (CState*)(new CSchoolGirlPursue(m_NPC, m_NextState));
			m_Stage = EVENT::EXIT;
			return;
		}
	}

	CSchoolGirl* girl = (CSchoolGirl*)m_NPC;

	if (!cannotAttack)
	{
		if (m_NPC->IsJumping())
			girl->JumpKnee();
		else
		{
			int branch = rand() % (int)SchoolGirlAttackType::MAX;

			switch ((SchoolGirlAttackType)branch)
			{
			case CSchoolGirlAttack::CRESENTKICK:
				girl->CresentKick();
				break;
			case CSchoolGirlAttack::JAB:
				girl->Jab();
				break;
			case CSchoolGirlAttack::KICK:
				girl->Kick();
				break;
			case CSchoolGirlAttack::TORNADOKICK:
				girl->TornadoKick();
				break;
			}
		}
	}
}

bool CSchoolGirlAttack::CannotAttackPlayer()
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
