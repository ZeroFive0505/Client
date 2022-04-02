#pragma once

#include "Game.h"

// 플레이어도 일종의 오브젝트 따라서 오브젝트 매니저의 관리를 받는다.

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

