#include "ObjectManager.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"

CObjectManager* CObjectManager::m_pInst = nullptr;

CObjectManager::CObjectManager() :
    m_pPlayer(nullptr),
    m_pMonster{}
{
}

CObjectManager::~CObjectManager()
{
    if (m_pPlayer)
    {
        delete  m_pPlayer;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (m_pMonster[i])
            delete  m_pMonster[i];
    }
}



CItem* CObjectManager::CreateItem(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
    CItem* Item = new CItem;

    Item->Init(Name, Type, Option, Price, Sell, Desc);

    return Item;
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

    // ����ġ ���̺��� �����.
    m_LevelUpExp[0] = 5000;
    m_LevelUpExp[1] = 12000;
    m_LevelUpExp[2] = 18000;
    m_LevelUpExp[3] = 25000;
    m_LevelUpExp[4] = 40000;
    m_LevelUpExp[5] = 60000;
    m_LevelUpExp[6] = 80000;
    m_LevelUpExp[7] = 100000;
    m_LevelUpExp[8] = 120000;
    m_LevelUpExp[9] = 200000;

    m_DropItem[0] = CreateItem("���(����)", IT_Weapon, 80, 1000, 500, "������ ���� ��");
    m_DropItem[1] = CreateItem("õ����(����)", IT_Armor, 45, 1000, 500, "���� ��");

    m_DropItem[2] = CreateItem("BF.���(����)", IT_Weapon, 400, 8000, 4000, "���� �� ��");
    m_DropItem[3] = CreateItem("������ ��Ʈ(����)", IT_Armor, 200, 8000, 4000, "���� �ܴ��� ��Ʈ");

    m_DropItem[4] = CreateItem("�����Ǵ��(����)", IT_Weapon, 900, 40000, 20000, "ġ��Ÿ �°� �׾��.");
    m_DropItem[5] = CreateItem("���ð���(����)", IT_Armor, 500, 40000, 20000, "������!!");


    return true;
}

CMonster* CObjectManager::CloneMonster(const char* pName)
{
    // �迭 ��ü ����ŭ �ݺ��ϸ� �̸��� ���ؼ� ���͸� ã�´�.
    CMonster* pFindMonster = nullptr;

    for (int i = 0; i < 3; ++i)
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

CItem* CObjectManager::CloneItem(int Index)
{
    return m_DropItem[Index]->Clone();
}
