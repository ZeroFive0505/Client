#pragma once

#include "Game.h"

// �÷��̾ ������ ������Ʈ ���� ������Ʈ �Ŵ����� ������ �޴´�.

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

private:
    char    m_Name[32];
    Job     m_Job;
    int     m_Attack;
    int     m_Armor;
    int     m_HP;
    int     m_HPMax;
    int     m_MP;
    int     m_MPMax;
    int     m_Level;
    int     m_Exp;
    int     m_Gold;

public:
    bool Init();
};

