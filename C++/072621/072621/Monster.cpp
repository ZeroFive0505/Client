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
    // ��� �����͸� ���� ����� �������ش�.
    // �ֳ��ϸ� ��� �����߿��� �����Ҵ��� ���� ������ ����. ���� ���� ����� ����� �����ϴ�.
    *this = monster;
}

CMonster::~CMonster()
{
}

bool CMonster::Init(const char* pName, int Attack, int Armor, int HP, int MP, int Level, int Exp, int Gold)
{
    // ���ڿ��� �����Ѵ�.
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
	// �� �Լ��� ȣ���� ��ü�� ������ ���ο� ��ü�� ���� ��ȯ�Ѵ�.
    return new CMonster(*this);
}
