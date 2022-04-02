#include "Monster.h"

CMonster::CMonster() :
    m_Name{},
    m_Attack(0),
    m_Armor(0),
    m_HP(0),
    m_HPMax(0),
    m_MP(0),
    m_MPMax(0),
    m_Level(0),
    m_Exp(0),
    m_Gold(0)
{
}

CMonster::CMonster(const CMonster& monster)
{
    // 모든 데이터를 얕은복사로 복사해준다.
    *this = monster;
}

CMonster::~CMonster()
{
}

bool CMonster::Init(const char* pName, int Attack, int Armor,
    int HP, int MP, int Level, int Exp, int Gold)
{
    // 문자열을 복사한다.
    strcpy_s(m_Name, pName);
    m_Attack = Attack;
    m_Armor = Armor;
    m_HP = HP;
    m_HPMax = HP;
    m_MP = MP;
    m_MPMax = MP;
    m_Level = Level;
    m_Exp = Exp;
    m_Gold = Gold;

    return true;
}

CMonster* CMonster::Clone()
{
    // 이 함수를 호출한 객체를 복사한 새로운 객체를 만들어서 반환한다.
    return new CMonster(*this);
}

void CMonster::Output()
{
    std::cout << "이름 : " << m_Name << "\n";
    std::cout << "레벨 : " << m_Level << "\t경험지 : " << m_Exp << "\n";
    std::cout << "공격력 : " << m_Attack << "\t방어력" << m_Armor << "\n";
    std::cout << "체력 : " << m_HP << " / " << m_HPMax << "\t마나 : " <<
        m_MP << " / " << m_MPMax << "\n";
    std::cout << "획득금액 : " << m_Gold << "\n";
}

bool CMonster::Damage(int Damage)
{
    m_HP -= Damage;

    if (m_HP <= 0)
        return true;

    return false;
}
