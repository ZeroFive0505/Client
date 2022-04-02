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
	std::cout << "�̸� : " << m_Name << "\t���� : ";
	switch (m_Type)
	{
	case IT_Weapon:
		std::cout << " ����\n";
		std::cout << "���ݷ� : " << m_Option << "\n";
		break;
	case IT_Armor:
		std::cout << " ��\n";
		std::cout << "���� : " << m_Option << "\n";
		break;
	}

	std::cout << "���� ���� : " << m_Price << "\t�ǸŰ��� : " << m_Sell << "\n";
	std::cout << "���� : " << m_Desc << "\n";
}

CItem* CItem::Clone()
{
	return  new CItem(*this);
}
