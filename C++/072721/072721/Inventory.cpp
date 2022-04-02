#include "Inventory.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Item.h"

CInventory* CInventory::m_pInst = nullptr;

CInventory::CInventory() : m_Count(0), m_Item{}
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < m_Count; i++)
	{
		if (m_Item[i])
			delete m_Item[i];
	}
}

void CInventory::RemoveItem(int Index)
{
	for (int i = Index; i < m_Count - 1; i++)
	{
		m_Item[i] = m_Item[i + 1];
	}

	m_Item[m_Count - 1] = nullptr;
	m_Count--;
}

void CInventory::OutputInventory()
{
	std::cout << "============ 소유하고 있는 아이템 목록 ============\n";
	for (int i = 0; i < m_Count; i++)
	{
		std::cout << i + 1 << ". ";
		m_Item[i]->Output();
	}
}

int CInventory::Menu(class CPlayer* Player)
{
	system("cls");

	std:: cout << "====================== 인벤토리 ======================\n";

	for (int i = 0; i < m_Count; i++)
	{
		std::cout << i + 1 << ". ";
		m_Item[i]->Output();
		std::cout << "\n";
	}

	std::cout << "0. 뒤로가기\n";
	std::cout << "장착 무기 : ";

	CItem* EquipItem = Player->GetEquipItem(Equip_Weapon);
	
	if (EquipItem)
		std::cout << EquipItem->GetName();
	else
		std::cout << "없음";

	std::cout << "\t장착방어구 : ";
	EquipItem = Player->GetEquipItem(Equip_Armor);

	if (EquipItem)
		std::cout << EquipItem->GetName() << "\n";
	else
		std::cout << "없음\n";

	std::cout << "장착할 아이템을 선택하세요 : ";
	
	int Item = 0;

	std::cin >> Item;

	if (Item < 0 || Item > m_Count)
		return -1;

	return Item;
}

bool CInventory::Init()
{
	return true;
}

void CInventory::Run()
{
	CPlayer* Player = CObjectManager::GetInst()->GetPlayer();

	while (true)
	{
		int Select = Menu(Player);

		if (Select == 0)
			break;
		else if (Select == -1)
			continue;

		int	Index = Select - 1;

		CItem* EquipItem = Player->Equip(m_Item[Index]);

		if (EquipItem)
			m_Item[Index] = EquipItem;
		else
		{
			// 장착시킨 아이템 뒤에 있는 아이템을 모두 1칸씩 앞으로 이동시켜준다.
			for (int i = Index; i < m_Count - 1; i++)
			{
				m_Item[i] = m_Item[i + 1];
			}

			// 가장 마지막 칸은 nullptr로 해준다.
			m_Item[m_Count - 1] = nullptr;
			m_Count--;
		}
	}
}
