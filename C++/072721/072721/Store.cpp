#include "Store.h"
#include "Item.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Inventory.h"

CStore::CStore() : m_Type(ST_Weapon), m_Item{}
{
}

CStore::~CStore()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_Item[i])
		{
			delete m_Item[i];
		}
	}
}

int CStore::Menu(CPlayer* Player)
{
	system("cls");
	switch (m_Type)
	{
	case ST_Weapon:
		std::cout << "===================== ���� ���� =====================\n";
		break;
	case ST_Armor:
		std::cout << "===================== �� ���� =====================\n";
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		std::cout << i + 1 << ". ";
		m_Item[i]->Output();
		std::cout << "\n";
	}

	std::cout << "4. �Ǹ� �޴�\n";
	std::cout << "5. �ڷΰ���\n";
	std::cout << "���� �ݾ� : " << Player->GetGold() << "\n";
	std::cout << "������ �������� �����ϼ��� : ";
	
	int Item;

	std::cin >> Item;

	if (Item < 1 || Item > 5)
		return 0;

	return Item;
}

CItem* CStore::CreateItem(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	CItem* Item = new CItem;

	Item->Init(Name, Type, Option, Price, Sell, Desc);

	return Item;
}

void CStore::SetWeaponList()
{
	m_Item[0] = CreateItem("���", IT_Weapon, 50, 1000, 500, "������ ���� ��");
	m_Item[1] = CreateItem("BF.���", IT_Weapon, 200, 8000, 4000, "���� �� ��");
	m_Item[2] = CreateItem("������ ���", IT_Weapon, 600, 40000, 20000, "ġ��Ÿ �°� �׾��.");
}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("õ ����", IT_Armor, 25, 1000, 500, "���� ����");
	m_Item[1] = CreateItem("������ ��Ʈ", IT_Armor, 100, 8000, 4000, "���� �ܴ��� ��Ʈ");
	m_Item[2] = CreateItem("���� ����", IT_Armor, 300, 40000, 20000, "������.");
}

bool CStore::Init(Store_Type Type)
{
    m_Type = Type;

	switch (m_Type)
	{
	case IT_Weapon:
		SetWeaponList();
		break;
	case IT_Armor:
		SetArmorList();
		break;
	}

    return true;
}

void CStore::Run()
{
	CPlayer* Player = CObjectManager::GetInst()->GetPlayer();


	// ���⿡�� �������� �Ǹ��� ������ ����� ����� �޽����� �ۼ�
	while (true)
	{
		int choice = Menu(Player);

		if (choice == 0)
			continue;
		else if (choice == 5)
			break;

		int Index = choice - 1;

		if (choice == 4)
		{	
			int maxCnt = CInventory::GetInst()->GetInventoryCount();

			if (maxCnt == 0)
			{
				std::cout << "�Ǹ��� �������� �����ϴ�. \n";
				system("pause");
				break;
			}


			do
			{
				system("cls");
				CInventory::GetInst()->OutputInventory();
				std::cout << "���� : (0 ���)";
				std::cin >> choice;

				if (choice == 0)
					return;

			} while (choice < 1 || choice > maxCnt);

			CItem* sellItem = CInventory::GetInst()->GetItem(choice - 1);

			std::cout << sellItem->GetName() << " �Ǹ� �Ϸ� \n";
			std::cout << sellItem->GetSell() << " ��� ȹ�� \n";
			Player->AddGold(sellItem->GetSell());

			CInventory::GetInst()->RemoveItem(choice - 1);
		}
		else
		{
			// �������� ������ �� �ִ��� �Ǵ��ؾ� �Ѵ�.
			// �������� ������ �� ���� ���� �÷��̾ ���� ���� �����ϰų�
			// �κ��丮�� �� �̻� ������ ���� ����̴�.
			if (Player->GetGold() < m_Item[Index]->GetPrice())
			{
				std::cout << "�ݾ��� �����մϴ�.\n";
				system("pause");
				continue;
			}
			else if (CInventory::GetInst()->IsFull())
			{
				std::cout << "������ �����մϴ�. ĳ���� �κ��丮�� �÷��ּ���.\n";
				system("pause");
				continue;
			}

			// ������� ������ �������� ������ �� �ִٴ� ���� �ȴ�.
			CItem* CloneItem = m_Item[Index]->Clone();

			// �÷��̾��� ���� ���ҽ�Ų��.
			Player->AddGold(-m_Item[Index]->GetPrice());

			// �κ��丮�� ������ �������� �߰����ش�.
			CInventory::GetInst()->AddItem(CloneItem);

			std::cout << "���� �Ϸ�\n";
		}

		system("pause");
	}
}
