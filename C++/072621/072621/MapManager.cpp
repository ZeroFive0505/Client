#include "MapManager.h"
#include "Map.h"

CMapManager* CMapManager::m_pInst = nullptr;

// ������ �迭 �ʱ�ȭ
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
	std::cout << "1. ����\n";
	std::cout << "2. ����\n";
	std::cout << "3. �����\n";
	std::cout << "4. �ڷΰ���\n";
	std::cout << "�޴��� �����ϼ��� : ";
	int _Menu = 0;

	std::cin >> _Menu;

	if (_Menu <= (int)Map_Menu::None || _Menu > (int)Map_Menu::Back)
		return Map_Menu::None;

	return (Map_Menu)_Menu;
}

bool CMapManager::Init()
{
	// �ʱ�ȭ�ÿ� ���� �����Ѵ�.

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
