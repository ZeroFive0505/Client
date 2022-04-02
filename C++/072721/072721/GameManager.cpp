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
    // �޸𸮸� �����Ѵ�.
    CMapManager::DestroyInst();

    CObjectManager::DestroyInst();

    CStoreManager::DestroyInst();

    CInventory::DestroyInst();
}

Main_Menu CGameManager::Menu()
{
    system("cls");
    std::cout << "1. ��" << std::endl;
    std::cout << "2. ����" << std::endl;
    std::cout << "3. �κ��丮" << std::endl;
    std::cout << "4. ����" << std::endl;
    std::cout << "�޴��� �����ϼ��� : ";
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

    // ������Ʈ ������ �ʱ�ȭ
    if (!CObjectManager::GetInst()->Init())
        return false;

    // ���������� �ʱ�ȸ
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
