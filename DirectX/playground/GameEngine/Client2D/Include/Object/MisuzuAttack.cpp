#include "MisuzuAttack.h"
#include "MisuzuPursue.h"
#include "MisuzuFallback.h"
#include "Misuzu.h"
#include "Resource/ResourceManager.h"

CMisuzuAttack::CMisuzuAttack(CRCGEnemy* npc, CState* nextState) :
	CState(npc, nextState)
{
	m_State = STATE::ATTACK;
}

CMisuzuAttack::~CMisuzuAttack()
{
}

void CMisuzuAttack::Update()
{
	if (m_NPC->OnGuard())
		return;

	if (m_NPC->CheckState(EEnemyState::DOWN))
		return;

	CMisuzu* misuzu = (CMisuzu*)m_NPC;

	if (misuzu->GetCurrentMove() == EMisuzuMoveSet::TACKLE)
	{
		Tackle();
		return;
	}
	else if (misuzu->GetCurrentMove() == EMisuzuMoveSet::METEOR)
	{
		Meteor();
		return;
	}
	else if (m_NPC->CheckState(EEnemyState::ATTACK))
		return;

	if (m_NPC->IsInvincible())
		return;

	float prob = (float)rand() / (float)RAND_MAX;

	if (prob > 0.05f)
		return;

	prob = (float)rand() / (float)RAND_MAX;


	if (prob <= 0.05f)
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)new CMisuzuFallback(m_NPC, m_NextState);
		m_Stage = EVENT::EXIT;
		return;
	}

	CRCGPlayer* player = (CRCGPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	bool cannotAttack = CannotAttackPlayer();

	if (cannotAttack)
	{
		prob = (float)rand() / (float)RAND_MAX;

		if (prob <= 0.2f)
		{
			SAFE_DELETE(m_NextState);
			m_NextState = (CState*)new CMisuzuPursue(m_NPC, m_NextState);
			m_Stage = EVENT::EXIT;
			return;
		}
	}

	if (!cannotAttack)
	{
		EMisuzuPhase phase = misuzu->GetMisuzuPhase();

		int branch = rand() % (int)EMisuzuAttackType::MAX;

		switch ((EMisuzuAttackType)branch)
		{
		case EMisuzuAttackType::ELBOW:
			misuzu->Elbow();
			break;
		case EMisuzuAttackType::SLAP:
			misuzu->Slap();
			break;
		}
	}
}

bool CMisuzuAttack::CannotAttackPlayer()
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

void CMisuzuAttack::Tackle()
{
	CMisuzu* misuzu = (CMisuzu*)m_NPC;

	if(!misuzu->IsTackleFollowing())
		return;

	m_LerpTime += 1.0f / 60.0f;

	CGameObject* player = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	Vector3 dir = player->GetWorldPos() - m_NPC->GetWorldPos();
	Vector3 tackleDir = misuzu->GetTackleDirection();
	tackleDir.Normalize();
	dir.Normalize();

	float followSpeed = m_NPC->GetWalkSpeed();

	if (m_LerpTime >= 1.0f)
	{
		misuzu->SetTackleDirection(dir);
		m_LerpTime = 0.0f;

		m_NPC->AddWorldPos(dir * followSpeed * 1.25f * 1.0f / 60.0f);

		if (m_NPC->IsFacingRight() && dir.x < 0.0f)
		{
			m_NPC->GetSpriteComponent()->ChangeAnimation("Tackle_turn");
		}
		else if (!m_NPC->IsFacingRight() && dir.x > 0.0f)
		{
			m_NPC->GetSpriteComponent()->ChangeAnimation("Tackle_turn");
		}
	}
	else
	{
		Vector3 lerpDir;

		float a = tackleDir.x;
		float b = dir.x;

		float t = a + m_LerpTime * (b - a);

		lerpDir.x = t;

		a = tackleDir.y;
		b = dir.y;

		t = a + m_LerpTime * (b - a);

		lerpDir.y = t;

		lerpDir.z = 0.0f;

		lerpDir.Normalize();

		m_NPC->AddWorldPos(lerpDir * followSpeed * 1.25f * 1.0f / 60.0f);

		if (m_NPC->IsFacingRight() && lerpDir.x < 0.0f)
			m_NPC->GetSpriteComponent()->ChangeAnimation("Tackle_turn");
		else if (!m_NPC->IsFacingRight() && lerpDir.x > 0.0f)
			m_NPC->GetSpriteComponent()->ChangeAnimation("Tackle_turn");
	}
}

void CMisuzuAttack::Meteor()
{
	CMisuzu* misuzu = (CMisuzu*)m_NPC;

	if (!misuzu->IsMeteorFollowing())
		return;

	float followSpeed = m_NPC->GetWalkSpeed();

	CGameObject* player = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	Vector3 dir = player->GetWorldPos() - m_NPC->GetWorldPos();

	dir.Normalize();

	m_NPC->AddWorldPos(dir * followSpeed * 2.0f * 1.0f / 60.0f);
}
