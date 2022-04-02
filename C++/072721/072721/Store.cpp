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
		std::cout << "===================== 무기 상점 =====================\n";
		break;
	case ST_Armor:
		std::cout << "===================== 방어구 상점 =====================\n";
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		std::cout << i + 1 << ". ";
		m_Item[i]->Output();
		std::cout << "\n";
	}

	std::cout << "4. 판매 메뉴\n";
	std::cout << "5. 뒤로가기\n";
	std::cout << "보유 금액 : " << Player->GetGold() << "\n";
	std::cout << "구매할 아이템을 선택하세요 : ";
	
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
	m_Item[0] = CreateItem("목검", IT_Weapon, 50, 1000, 500, "나무로 만든 검");
	m_Item[1] = CreateItem("BF.대검", IT_Weapon, 200, 8000, 4000, "조금 센 검");
	m_Item[2] = CreateItem("무한의 대검", IT_Weapon, 600, 40000, 20000, "치명타 맞고 죽어라.");
}

void CStore::SetArmorList()
{
	m_Item[0] = CreateItem("천 갑옷", IT_Armor, 25, 1000, 500, "약한 갑옷");
	m_Item[1] = CreateItem("거인의 벨트", IT_Armor, 100, 8000, 4000, "조금 단단한 벨트");
	m_Item[2] = CreateItem("가시 갑옷", IT_Armor, 300, 40000, 20000, "구른다.");
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


	// 여기에서 상점에서 판매할 아이템 목록을 출력할 메시지를 작성
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
				std::cout << "판매할 아이템이 없습니다. \n";
				system("pause");
				break;
			}


			do
			{
				system("cls");
				CInventory::GetInst()->OutputInventory();
				std::cout << "선택 : (0 취소)";
				std::cin >> choice;

				if (choice == 0)
					return;

			} while (choice < 1 || choice > maxCnt);

			CItem* sellItem = CInventory::GetInst()->GetItem(choice - 1);

			std::cout << sellItem->GetName() << " 판매 완료 \n";
			std::cout << sellItem->GetSell() << " 골드 획득 \n";
			Player->AddGold(sellItem->GetSell());

			CInventory::GetInst()->RemoveItem(choice - 1);
		}
		else
		{
			// 아이템을 구매할 수 있는지 판단해야 한다.
			// 아이템을 구매할 수 없는 경우는 플레이어가 가진 돈이 부족하거나
			// 인벤토리에 더 이상 공간이 없을 경우이다.
			if (Player->GetGold() < m_Item[Index]->GetPrice())
			{
				std::cout << "금액이 부족합니다.\n";
				system("pause");
				continue;
			}
			else if (CInventory::GetInst()->IsFull())
			{
				std::cout << "공간이 부족합니다. 캐쉬로 인벤토리를 늘려주세요.\n";
				system("pause");
				continue;
			}

			// 여기까지 왔으면 아이템을 구매할 수 있다는 것이 된다.
			CItem* CloneItem = m_Item[Index]->Clone();

			// 플레이어의 돈을 감소시킨다.
			Player->AddGold(-m_Item[Index]->GetPrice());

			// 인벤토리에 구매한 아이템을 추가해준다.
			CInventory::GetInst()->AddItem(CloneItem);

			std::cout << "구매 완료\n";
		}

		system("pause");
	}
}
