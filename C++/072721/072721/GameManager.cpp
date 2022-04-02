#include "GameManager.h"
#include "MapManager.h"
#include "ObjectManager.h"
#include "StoreManager.h"
#include "Inventory.h"

CGameManager* CGameManager::m_pInst = nullptr;

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
    // 메모리를 해제한다.
    CMapManager::DestroyInst();

    CObjectManager::DestroyInst();

    CStoreManager::DestroyInst();

    CInventory::DestroyInst();
}

Main_Menu CGameManager::Menu()
{
    system("cls");
    std::cout << "1. 맵" << std::endl;
    std::cout << "2. 상점" << std::endl;
    std::cout << "3. 인벤토리" << std::endl;
    std::cout << "4. 종료" << std::endl;
    std::cout << "메뉴를 선택하세요 : ";
    int _Menu = 0;

    std::cin >> _Menu;

    if (_Menu <= (int)Main_Menu::None || _Menu > (int)Main_Menu::Exit)
        return Main_Menu::None;

    return (Main_Menu)_Menu;
}

bool CGameManager::Init()
{
    if (!CMapManager::GetInst()->Init())
        return false;

    // 오브젝트 관리자 초기화
    if (!CObjectManager::GetInst()->Init())
        return false;

    // 상점관리자 초기회
    if (!CStoreManager::GetInst()->Init())
        return false;

    if (!CInventory::GetInst()->Init())
        return false;

    return true;
}

void CGameManager::Run()
{
    while (true)
    {
        switch (Menu())
        {
        case Main_Menu::Map:
            CMapManager::GetInst()->Run();
            break;
        case Main_Menu::Store:
            CStoreManager::GetInst()->Run();
            break;
        case Main_Menu::Inventory:
            CInventory::GetInst()->Run();
            break;
        case Main_Menu::Exit:
            return;
        }
    }
}
