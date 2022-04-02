#include "GameManager.h"
#include "MapManager.h"
#include "ObjectManager.h"

CGameManager* CGameManager::m_pInst = nullptr;

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
	// ���ӸŴ����� ��� �Ŵ������� �����Ѵ�. ���� ���ӸŴ����� �ı��ɶ�
	// ��� �Ŵ����鵵 �ı��Ѵ�.
	CMapManager::DestroyInst();

	CObjectManager::DestroyInst();
}

Main_Menu CGameManager::Menu()
{
	system("cls");
	std::cout << "1. ��\n";
	std::cout << "2. ����\n";
	std::cout << "3. �κ��丮\n";
	std::cout << "4. ����\n";
	std::cout << "�޴��� �����ϼ��� : ";

	int _Menu = 0;

	std::cin >> _Menu;

	if (_Menu <= (int)Main_Menu::None || _Menu > (int)Main_Menu::Exit)
		return Main_Menu::None;

	return (Main_Menu)_Menu;
}

bool CGameManager::Init()
{
	// �ı��ڿ� ���������� �ʱ�ȭ�ÿ��� ��� �Ŵ����鵵 �ʱ�ȭ �Ѵ�.

	// ���� �ʱ�ȭ�� ���������ÿ� false�� ��ȯ�ǰ� ������ ����
	if (!CMapManager::GetInst()->Init())
		return false;

	if (!CObjectManager::GetInst()->Init())
		return false;

	return true;
}

void CGameManager::Run()
{
	// Run�� ���� �ݺ��� ���鼭 ������ ������ ���� ��Ų��.
	while (true)
	{
		switch (Menu())
		{
		case Main_Menu::Map:
			// ���� ����� ��� �� �Ŵ��� �̱����� �ҷ��ͼ� ����
			CMapManager::GetInst()->Run();
			break;
		case Main_Menu::Store:
			break;
		case Main_Menu::Inventory:
			break;
		case Main_Menu::Exit:
			// ��ȯ���� ���� �ݺ����� ���������� �� main.cpp�� ���ư��� �̱����� �ı��ϰ� �����Ѵ�.
			return;
		}
	}
}
