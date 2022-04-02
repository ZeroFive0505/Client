#include "Player.h"

CPlayer::CPlayer() :
	m_Name{},
	m_Job(Job::None),
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

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	std::cout << "이름을 입력하세요 : ";
	std::cin >> m_Name;

	int PlayerJob;

	while (true)
	{
		system("cls");
		std::cout << "1. 기사\n";
		std::cout << "2. 궁수\n";
		std::cout << "3. 마법사\n";
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
