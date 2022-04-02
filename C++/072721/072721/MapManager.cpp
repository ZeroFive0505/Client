#include "MapManager.h"
#include "Map.h"

CMapManager* CMapManager::m_pInst = nullptr;

CMapManager::CMapManager() :
    m_pMap{}        // 포인터 배열을 모두 nullptr(0)로 초기화한다.
{
}

CMapManager::~CMapManager()
{
    for (int i = 0; i < MT_End; ++i)
    {
        // m_pMap[i] 가 nullptr이 아니라면 true로 인식된다.
        if (m_pMap[i])
        {
            delete  m_pMap[i];
        }
    }
}

Map_Menu CMapManager::Menu()
{
    system("cls");
    std::cout << "1. 쉬움" << std::endl;
    std::cout << "2. 보통" << std::endl;
    std::cout << "3. 어려움" << std::endl;
    std::cout << "4. 뒤로가기" << std::endl;
    std::cout << "메뉴를 선택하세요 : ";
    int _Menu = 0;

    std::cin >> _Menu;

    if (_Menu <= (int)Map_Menu::None || _Menu > (int)Map_Menu::Back)
        return Map_Menu::None;

    return (Map_Menu)_Menu;
}

bool CMapManager::Init()
{
    // Map을 생성한다.
    for (int i = 0; i < MT_End; ++i)
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
