#include "MapManager.h"
#include "Map.h"

CMapManager* CMapManager::m_pInst = nullptr;

// 포인터 배열 초기화
CMapManager::CMapManager() : m_pMap{}
{
}

CMapManager::~CMapManager()
{
	for (int i = 0; i < MT_End; i++)
	{
		if (m_pMap[i])
		{
			delete m_pMap[i];
		}
	}
}

Map_Menu CMapManager::Menu()
{
	system("cls");
	std::cout << "1. 쉬움\n";
	std::cout << "2. 보통\n";
	std::cout << "3. 어려움\n";
	std::cout << "4. 뒤로가기\n";
	std::cout << "메뉴를 선택하세요 : ";
	int _Menu = 0;

	std::cin >> _Menu;

	if (_Menu <= (int)Map_Menu::None || _Menu > (int)Map_Menu::Back)
		return Map_Menu::None;

	return (Map_Menu)_Menu;
}

bool CMapManager::Init()
{
	// 초기화시에 맵을 생성한다.

	for (int i = 0; i < MT_End; i++)
	{
		m_pMap[i] = new CMap;

		if (!m_pMap[i]->Init((Map_Type)i))
		{
			return false;
		}
	}

	return true;
}

void CMapManager::Run()
{
	while (true)
	{
		switch (Menu())
		{
		case Map_Menu::Easy:
			m_pMap[0]->Run();
			break;
		case Map_Menu::Normal:
			m_pMap[1]->Run();
			break;
		case Map_Menu::Hard:
			m_pMap[2]->Run();
			break;
		case Map_Menu::Back:
			return;
		}
	}
}
