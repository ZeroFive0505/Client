#pragma once

#include "Game.h"

class CItem
{
public:
	CItem();
	CItem(const CItem& item);
	~CItem();

private:
	char m_Name[32];
	Item_Type m_Type;
	int m_Option;
	int m_Price;
	int m_Sell;
	char m_Desc[512];

public:
	const char* GetName()
	{
		return m_Name;
	}

	Item_Type GetItemType()
	{
		return m_Type;
	}

	int GetOption()
	{
		return m_Option;
	}

	int GetPrice()
	{
		return m_Price;
	}

	int GetSell()
	{
		return m_Sell;
	}

	const char* GetItemDesc()
	{
		return m_Desc;
	}

public:
	bool Init(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc);
	void Output();
	CItem* Clone();
};

