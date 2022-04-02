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
        return CObjectManager::GetInst()->CloneMonster("���");
    case MT_Normal:
        return CObjectManager::GetInst()->CloneMonster("��ũ");
        break;
    case MT_Hard:
        return CObjectManager::GetInst()->CloneMonster("�巡��");
        break;
    }

    return nullptr;
}

Battle_Menu CMap::Menu()
{
    std::cout << "1. ����" << std::endl;
    std::cout << "2. ����" << std::endl;
    std::cout << "�޴��� �����ϼ��� : ";
    int _Menu = 0;

    std::cin >> _Menu;

    if (_Menu <= (int)Battle_Menu::None || _Menu > (int)Battle_Menu::Back)
        return Battle_Menu::None;

    return (Battle_Menu)_Menu;
}

Battle_Result CMap::Battle(CPlayer* pPlayer, CMonster* pMonster)
{
    // ������ ���
    int Attack = pPlayer->GetAttack();
    int Armor = pPlayer->GetArmor();
    if (pPlayer->GetEquipItem(Equip_Weapon))
        Attack += pPlayer->GetEquipItem(Equip_Weapon)->GetOption();

    if (pPlayer->GetEquipItem(Equip_Armor))
        Armor += pPlayer->GetEquipItem(Equip_Armor)->GetOption();

    int Damage = Attack - pMonster->GetArmor();

    // ? ������ ������ ���� ��� ���� : �����ʿ��� ���� ������ ��� ������
    Damage = Damage < 1 ? 1 : Damage;

    // ���Ͱ� �׾��� ���.
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

        // �÷��̾� ������ ����Ѵ�.
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

                        std::cout << DropItem->GetName() << " Item ȹ��\n";
                    }
                }

                // ���͸� �����ϰ� ���� �����Ѵ�.
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
            // ���ʹ� ������ ������ ���ο� ���͸� �����Ҵ��ؼ� ������⿡
            // �޸� ����
            delete pMonster;
            return;
            break;
        }
    }
}
