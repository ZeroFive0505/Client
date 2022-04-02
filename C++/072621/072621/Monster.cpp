#include "Monster.h"

CMonster::CMonster() :
    m_Name{},
    m_Armor(0),
    m_Attack(0),
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
    // 모든 데이터를 얕은 복사로 복사해준다.
    // 왜냐하면 멤버 변수중에는 동적할당을 받은 변수는 없다. 따라서 얕은 복사로 충분히 가능하다.
    *this = monster;
}

CMonster::~CMonster()
{
}

bool CMonster::Init(const char* pName, int Attack, int Armor, int HP, int MP, int Level, int Exp, int Gold)
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
