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
	// �Ŵ����� �ı��ɶ� �����ϴ� ������Ʈ�鵵 ���� �ı�
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
	// �÷��̾ �����Ѵ�.
	m_pPlayer = new CPlayer;

	if (!m_pPlayer->Init())
		return false;

	m_pMonster[0] = new CMonster;
	m_pMonster[0]->Init("���", 60, 5, 100, 10, 1, 1000, 500);

	m_pMonster[1] = new CMonster;
	m_pMonster[1]->Init("��ũ", 200, 100, 1000, 100, 5, 6000, 2500);

	m_pMonster[2] = new CMonster;
	m_pMonster[2]->Init("�巡��", 500, 500, 10000, 10000, 10, 20000,
		10000);

	return true;
}

CMonster* CObjectManager::CloneMonster(const char* pName)
{
	// �迭�� ��ü �� ��ũ �ݺ��ϸ� �̸��� �� �� ���͸� ã�´�.
	CMonster* pFindMonster = nullptr;

	for (int i = 0; i < 3; i++)
	{
		// strcmp : 1�����ڿ� 2�����ڷ� ���� ���ڿ��� �����ִ� �Լ��̴�.
	   // �� ���ڿ��� ���� ��� 0�� ��ȯ���ش�.
		if (strcmp(m_pMonster[i]->GetName(), pName) == 0)
		{
			pFindMonster = m_pMonster[i];
			break;
		}
	}

	// pFindMonster �� nullptr�̶�� ���� for������ ���ϴ� ���͸�
	// ��ã�� ���̹Ƿ� nullptr�� ��ȯ���ֵ��� �Ѵ�.
	if (pFindMonster == nullptr)
		return nullptr;

	return pFindMonster->Clone();
}
