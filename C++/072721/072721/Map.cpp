#include "Map.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "Inventory.h"
#include "Item.h"

CMap::CMap()
{
}

CMap::~CMap()
{
}

CMonster* CMap::SpawnMonster()
{
    switch (m_Type)
    {
    case MT_Easy:
        return CObjectManager::GetInst()->CloneMonster("고블린");
    case MT_Normal:
        return CObjectManager::GetInst()->CloneMonster("오크");
        break;
    case MT_Hard:
        return CObjectManager::GetInst()->CloneMonster("드래곤");
        break;
    }

    return nullptr;
}

Battle_Menu CMap::Menu()
{
    std::cout << "1. 공격" << std::endl;
    std::cout << "2. 도망" << std::endl;
    std::cout << "메뉴를 선택하세요 : ";
    int _Menu = 0;

    std::cin >> _Menu;

    if (_Menu <= (int)Battle_Menu::None || _Menu > (int)Battle_Menu::Back)
        return Battle_Menu::None;

    return (Battle_Menu)_Menu;
}

Battle_Result CMap::Battle(CPlayer* pPlayer, CMonster* pMonster)
{
    // 데미지 계산
    int Attack = pPlayer->GetAttack();
    int Armor = pPlayer->GetArmor();
    if (pPlayer->GetEquipItem(Equip_Weapon))
        Attack += pPlayer->GetEquipItem(Equip_Weapon)->GetOption();

    if (pPlayer->GetEquipItem(Equip_Armor))
        Armor += pPlayer->GetEquipItem(Equip_Armor)->GetOption();

    int Damage = Attack - pMonster->GetArmor();

    // ? 왼쪽의 조건이 참일 경우 왼쪽 : 오른쪽에서 왼쪽 거짓일 경우 오른쪽
    Damage = Damage < 1 ? 1 : Damage;

    // 몬스터가 죽었을 경우.
    if (pMonster->Damage(Damage))
    {
        return Battle_Result::Monster_Death;
    }

    Damage = pMonster->GetAttack() - Armor;

    Damage = Damage < 1 ? 1 : Damage;

    if (pPlayer->Damage(Damage))
    {
        return Battle_Result::Player_Death;
    }

    return Battle_Result::None;
}

bool CMap::Init(Map_Type Type)
{
    m_Type = Type;

    return true;
}

void CMap::Run()
{
    CPlayer* pPlayer = CObjectManager::GetInst()->GetPlayer();
    CMonster* pMonster = SpawnMonster();

    while (true)
    {
        system("cls");

        // 플레이어 정보를 출력한다.
        std::cout << "====================== Player ======================\n";
        pPlayer->Output();
        std::cout << "\n";
        std::cout << "====================== Monster ======================\n";
        pMonster->Output();
        std::cout << "\n";


        switch (Menu())
        {
        case Battle_Menu::Attack:
            switch (Battle(pPlayer, pMonster))
            {
            case Battle_Result::Monster_Death:
            {
                pPlayer->AddGold(pMonster->GetGold());
                pPlayer->AddExp(pMonster->GetExp());

                if (!CInventory::GetInst()->IsFull())
                {
                    float percent = rand() % 10001 / 100.0f;

                    if (percent < 10.0f)
                    {
                        int Index = -1;

                        if (rand() % 2 == 0)
                        {
                            Index = (int)m_Type * 2;
                        }
                        else
                        {
                            Index = (int)m_Type * 2 + 1;
                        }

                        CItem* DropItem = CObjectManager::GetInst()->CloneItem(Index);

                        CInventory::GetInst()->AddItem(DropItem);

                        std::cout << DropItem->GetName() << " Item 획득\n";
                    }
                }

                // 몬스터를 제거하고 새로 스폰한다.
                delete pMonster;

                pMonster = SpawnMonster();

                system("pause");
            }
                break;
            case Battle_Result::Player_Death:
                pPlayer->Death();
                break;
            }
            break;
        case Battle_Menu::Back:
            // 몬스터는 원본을 복제한 새로운 몬스터를 동적할당해서 만들었기에
            // 메모리 제거
            delete pMonster;
            return;
            break;
        }
    }
}
