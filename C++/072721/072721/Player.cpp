#include "Player.h"
#include "ObjectManager.h"
#include "Item.h"


CPlayer::CPlayer() :
    m_Name{},
    m_Attack(0),
    m_Job(Job::None),
    m_Armor(0),
    m_HP(0),
    m_HPMax(0),
    m_MP(0),
    m_MPMax(0),
    m_Level(0),
    m_Exp(0),
    m_Gold(0),
    m_EquipItem{}
{
}

CPlayer::~CPlayer()
{
}

CItem* CPlayer::Equip(CItem* Item)
{
    Equip_Type Type;

    switch (Item->GetItemType())
    {
    case IT_Weapon:
        Type = Equip_Weapon;
        break;
    case IT_Armor:
        Type = Equip_Armor;
        break;
    }

    CItem* EquipItem = m_EquipItem[Type];
    m_EquipItem[Type] = Item;

    // �������� �����ϰ� �������� ��ȯ�Ѵ�. ���ٸ� nullptr
    return EquipItem;
}

bool CPlayer::Init()
{
    // �̸��� �Է¹޴´�.
    std::cout << "�̸��� �Է��ϼ��� : ";
    std::cin >> m_Name;

    // ������ �����Ѵ�.
    int PlayerJob;

    while (true)
    {
        system("cls");
        std::cout << "1. ���" << std::endl;
        std::cout << "2. �ü�" << std::endl;
        std::cout << "3. ������" << std::endl;
        std::cout << "������ �����ϼ��� : ";
        std::cin >> PlayerJob;

        if ((int)Job::None < PlayerJob && PlayerJob < (int)Job::End)
            break;
    }

    m_Job = (Job)PlayerJob;

    switch (m_Job)
    {
    case Job::Knight:
        m_Attack = 20;
        m_Armor = 40;
        m_HP = 1000;
        m_HPMax = 1000;
        m_MP = 100;
        m_MPMax = 100;
        break;
    case Job::Archer:
        m_Attack = 30;
        m_Armor = 30;
        m_HP = 700;
        m_HPMax = 700;
        m_MP = 200;
        m_MPMax = 200;
        break;
    case Job::Magicion:
        m_Attack = 40;
        m_Armor = 20;
        m_HP = 500;
        m_HPMax = 500;
        m_MP = 300;
        m_MPMax = 300;
        break;
    }

    m_Level = 1;
    m_Gold = 10000;

    return true;
}

void CPlayer::Output()
{
    std::cout << "�̸� : " << m_Name << "\t���� : ";

    switch (m_Job)
    {
    case Job::Knight:
        std::cout << "���\n";
        break;
    case Job::Archer:
        std::cout << "�ü�\n";
        break;
    case Job::Magicion:
        std::cout << "������\n";
        break;
    }

    std::cout << "���� : " << m_Level << "\t������ : " << m_Exp << "\n";
    std::cout << "���ݷ� : " << m_Attack; 
    if (m_EquipItem[Equip_Weapon])
        std::cout << " + " << m_EquipItem[Equip_Weapon]->GetOption();

    std::cout << "\t���� : " << m_Armor;

    if (m_EquipItem[Equip_Armor])
        std::cout << " + " << m_EquipItem[Equip_Armor]->GetOption();
    
    std::cout << "\n";

    std::cout << "ü�� : " << m_HP << " / " << m_HPMax << "\t����: " <<
        m_MP << " / " << m_MPMax << "\n";
    std::cout << "�����ݾ� : " << m_Gold << "\n";

    std::cout << "���� ���� : ";

    if (m_EquipItem[Equip_Weapon])
        std::cout << m_EquipItem[Equip_Weapon]->GetName() << "\n";
    else
        std::cout << "����\n";

    std::cout << "���� �� : ";

    if (m_EquipItem[Equip_Armor])
        std::cout << m_EquipItem[Equip_Armor]->GetName() << "\n";
    else
        std::cout << "����\n";

}

bool CPlayer::Damage(int Damage)
{
    m_HP -= Damage;

    if (m_HP <= 0)
        return true;

    return false;
}

void CPlayer::AddGold(int Gold)
{
    m_Gold += Gold;
}

void CPlayer::AddExp(int Exp)
{
    m_Exp += Exp;

    // �������� �ʿ��� ����ġ�� ���´�.
    int LevelUpExp = CObjectManager::GetInst()->GetLevelUpExp(m_Level);

    while (m_Exp >= LevelUpExp)
    {
        if (m_Level >= LevelUpExp)
            m_Exp = LevelUpExp - 1;
        else
        {
            m_Level++;
            m_Exp -= LevelUpExp;

            // �ɷ�ġ�� �÷��ش�.
            switch (m_Job)
            {
            case Job::Knight:
                m_Attack += 10;
                m_Armor += 20;
                m_HP += 300;
                m_HPMax += 300;
                m_MP += 100;
                m_MPMax += 100;
                break;
            case Job::Archer:
                m_Attack += 10;
                m_Armor += 15;
                m_HP += 200;
                m_HPMax += 200;
                m_MP += 200;
                m_MPMax += 200;
                break;
            case Job::Magicion:
                m_Attack += 20;
                m_Armor += 10;
                m_HP += 100;
                m_HPMax += 100;
                m_MP += 300;
                m_MPMax += 300;
                break;
            }
        }
    }
}

void CPlayer::Death()
{
    m_Exp -= (int)(m_Exp * 0.1f);
    m_Gold -= (int)(m_Gold * 0.1f);

    m_HP = m_HPMax;
    m_MP = m_MPMax;
}
