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

    // 아이템을 장착하고 아이템을 반환한다. 없다면 nullptr
    return EquipItem;
}

bool CPlayer::Init()
{
    // 이름을 입력받는다.
    std::cout << "이름을 입력하세요 : ";
    std::cin >> m_Name;

    // 직업을 선택한다.
    int PlayerJob;

    while (true)
    {
        system("cls");
        std::cout << "1. 기사" << std::endl;
        std::cout << "2. 궁수" << std::endl;
        std::cout << "3. 마법사" << std::endl;
        std::cout << "직업을 선택하세요 : ";
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
    std::cout << "이름 : " << m_Name << "\t직업 : ";

    switch (m_Job)
    {
    case Job::Knight:
        std::cout << "기사\n";
        break;
    case Job::Archer:
        std::cout << "궁수\n";
        break;
    case Job::Magicion:
        std::cout << "마법사\n";
        break;
    }

    std::cout << "레벨 : " << m_Level << "\t경험지 : " << m_Exp << "\n";
    std::cout << "공격력 : " << m_Attack; 
    if (m_EquipItem[Equip_Weapon])
        std::cout << " + " << m_EquipItem[Equip_Weapon]->GetOption();

    std::cout << "\t방어력 : " << m_Armor;

    if (m_EquipItem[Equip_Armor])
        std::cout << " + " << m_EquipItem[Equip_Armor]->GetOption();
    
    std::cout << "\n";

    std::cout << "체력 : " << m_HP << " / " << m_HPMax << "\t마나: " <<
        m_MP << " / " << m_MPMax << "\n";
    std::cout << "보유금액 : " << m_Gold << "\n";

    std::cout << "장착 무기 : ";

    if (m_EquipItem[Equip_Weapon])
        std::cout << m_EquipItem[Equip_Weapon]->GetName() << "\n";
    else
        std::cout << "없음\n";

    std::cout << "장착 방어구 : ";

    if (m_EquipItem[Equip_Armor])
        std::cout << m_EquipItem[Equip_Armor]->GetName() << "\n";
    else
        std::cout << "없음\n";

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

    // 레벨업에 필요한 경험치를 얻어온다.
    int LevelUpExp = CObjectManager::GetInst()->GetLevelUpExp(m_Level);

    while (m_Exp >= LevelUpExp)
    {
        if (m_Level >= LevelUpExp)
            m_Exp = LevelUpExp - 1;
        else
        {
            m_Level++;
            m_Exp -= LevelUpExp;

            // 능력치를 올려준다.
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
