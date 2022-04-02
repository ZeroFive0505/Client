#include "Item.h"

CItem::CItem() :
	m_Name{},
	m_Type(IT_Weapon),
	m_Option(0),
	m_Price(0),
	m_Sell(0)
{
}

CItem::CItem(const CItem& item)
{
	*this = item;
}

CItem::~CItem()
{
}


bool CItem::Init(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc)
{
	strcpy_s(m_Name, Name);
	m_Type = Type;
	m_Option = Option;
	m_Price = Price;
	m_Sell = Sell;
	strcpy_s(m_Desc, Desc);

	return true;
}

void CItem::Output()
{
	std::cout << "이름 : " << m_Name << "\t종류 : ";
	switch (m_Type)
	{
	case IT_Weapon:
		std::cout << " 무기\n";
		std::cout << "공격력 : " << m_Option << "\n";
		break;
	case IT_Armor:
		std::cout << " 방어구\n";
		std::cout << "방어력 : " << m_Option << "\n";
		break;
	}

	std::cout << "상점 가격 : " << m_Price << "\t판매가격 : " << m_Sell << "\n";
	std::cout << "설명 : " << m_Desc << "\n";
}

CItem* CItem::Clone()
{
	return  new CItem(*this);
}
