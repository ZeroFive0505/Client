#include "MTBlock.h"

CMTBlock::CMTBlock(CRCGEnemy* npc, CState* nextState) :
    CState(npc, nextState)
{
    m_State = STATE::BLOCK;
}

CMTBlock::~CMTBlock()
{
}

void CMTBlock::Update()
{
	float prob = (float)rand() / (float)RAND_MAX;

	if (prob <= 0.05f && BlockEndCondition())
	{
		SAFE_DELETE(m_NextState);
		m_NextState = (CState*)(new CMTPursue(m_NPC, m_NextState));
		m_Stage = EVENT::EXIT;
		return;
	}
}

bool CMTBlock::BlockEndCondition()
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
	{
		return true;
	}
	else
	{
		CRCGPlayer* player = (CRCGPlayer*)obj;

		if (player->CheckState(EKyokoState::ATTACK))
			return false;
		else
			return true;
	}
}
