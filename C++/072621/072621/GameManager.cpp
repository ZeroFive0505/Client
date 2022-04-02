#include "GameManager.h"
#include "MapManager.h"
#include "ObjectManager.h"

CGameManager* CGameManager::m_pInst = nullptr;

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
	// 게임매니저가 모든 매니저들을 관리한다. 따라서 게임매니저가 파괴될때
	// 모든 매니저들도 파괴한다.
	CMapManager::DestroyInst();

	CObjectManager::DestroyInst();
}

Main_Menu CGameManager::Menu()
{
	system("cls");
	std::cout << "1. 맵\n";
	std::cout << "2. 상점\n";
	std::cout << "3. 인벤토리\n";
	std::cout << "4. 종료\n";
	std::cout << "메뉴를 선택하세요 : ";

	int _Menu = 0;

	std::cin >> _Menu;

	if (_Menu <= (int)Main_Menu::None || _Menu > (int)Main_Menu::Exit)
		return Main_Menu::None;

	return (Main_Menu)_Menu;
}

bool CGameManager::Init()
{
	// 파괴자와 마찬가지다 초기화시에는 모든 매니저들도 초기화 한다.

	// 만약 초기화가 실패했을시에 false가 반환되고 게임은 종료
	if (!CMapManager::GetInst()->Init())
		return false;

	if (!CObjectManager::GetInst()->Init())
		return false;

	return true;
}

void CGameManager::Run()
{
	// Run은 무한 반복을 돌면서 게임의 로직을 실행 시킨다.
	while (true)
	{
		switch (Menu())
		{
		case Main_Menu::Map:
			// 맵을 골랐을 경우 맵 매니저 싱글톤을 불러와서 실행
			CMapManager::GetInst()->Run();
			break;
		case Main_Menu::Store:
			break;
		case Main_Menu::Inventory:
			break;
		case Main_Menu::Exit:
			// 반환으로 무한 반복문을 빠져나간다 즉 main.cpp로 돌아가고 싱글톤을 파괴하고 종료한다.
			return;
		}
	}
}
