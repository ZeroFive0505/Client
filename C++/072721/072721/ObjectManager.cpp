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

    // 경험치 테이블을 만든다.
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

    m_DropItem[0] = CreateItem("목검(레어)", IT_Weapon, 80, 1000, 500, "나무로 만든 검");
    m_DropItem[1] = CreateItem("천갑옷(레어)", IT_Armor, 45, 1000, 500, "약한 방어구");

    m_DropItem[2] = CreateItem("BF.대검(레어)", IT_Weapon, 400, 8000, 4000, "조금 센 검");
    m_DropItem[3] = CreateItem("거인의 벨트(레어)", IT_Armor, 200, 8000, 4000, "조금 단단한 벨트");

    m_DropItem[4] = CreateItem("무한의대검(레어)", IT_Weapon, 900, 40000, 20000, "치명타 맞고 죽어라.");
    m_DropItem[5] = CreateItem("가시갑옷(레어)", IT_Armor, 500, 40000, 20000, "구른다!!");


    return true;
}

CMonster* CObjectManager::CloneMonster(const char* pName)
{
    // 배열 전체 수만큼 반복하며 이름을 비교해서 몬스터를 찾는다.
    CMonster* pFindMonster = nullptr;

    for (int i = 0; i < 3; ++i)
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

CItem* CObjectManager::CloneItem(int Index)
{
    return m_DropItem[Index]->Clone();
}
