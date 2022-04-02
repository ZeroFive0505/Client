#include "ObjectManager.h"
#include "Monster.h"
#include "Player.h"

CObjectManager* CObjectManager::m_pInst = nullptr;

CObjectManager::CObjectManager() :
	m_pPlayer(nullptr), m_pMonster{}
{
}

CObjectManager::~CObjectManager()
{
	// 매니저가 파괴될때 관리하던 오브젝트들도 전부 파괴
	if (m_pPlayer)
		delete m_pPlayer;
	
	for (int i = 0; i < 3; i++)
	{
		if (m_pMonster[i])
			delete m_pMonster[i];
	}
}

bool CObjectManager::Init()
{
	// 플레이어를 생성한다.
	m_pPlayer = new CPlayer;

	if (!m_pPlayer->Init())
		return false;

	m_pMonster[0] = new CMonster;
	m_pMonster[0]->Init("고블린", 60, 5, 100, 10, 1, 1000, 500);

	m_pMonster[1] = new CMonster;
	m_pMonster[1]->Init("오크", 200, 100, 1000, 100, 5, 6000, 2500);

	m_pMonster[2] = new CMonster;
	m_pMonster[2]->Init("드래곤", 500, 500, 10000, 10000, 10, 20000,
		10000);

	return true;
}

CMonster* CObjectManager::CloneMonster(const char* pName)
{
	// 배열의 전체 수 만크 반복하며 이름을 비교 후 몬스터를 찾는다.
	CMonster* pFindMonster = nullptr;

	for (int i = 0; i < 3; i++)
	{
		// strcmp : 1번인자와 2번인자로 들어가는 문자열을 비교해주는 함수이다.
	   // 두 문자열이 같을 경우 0을 반환해준다.
		if (strcmp(m_pMonster[i]->GetName(), pName) == 0)
		{
			pFindMonster = m_pMonster[i];
			break;
		}
	}

	// pFindMonster 가 nullptr이라면 위의 for문에서 원하는 몬스터를
	// 못찾은 것이므로 nullptr을 반환해주도록 한다.
	if (pFindMonster == nullptr)
		return nullptr;

	return pFindMonster->Clone();
}
