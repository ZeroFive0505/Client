#pragma once

#include "Game.h"

class CInventory
{
private:
    CInventory();
    ~CInventory();

private:
    class CItem* m_Item[INVENTORY_MAX];
    int m_Count;

public:
    int GetInventoryCount()
    {
        return m_Count;
    }

    bool IsFull()
    {
        return m_Count == INVENTORY_MAX;
    }

public:
    void AddItem(class CItem* Item)
    {
        m_Item[m_Count++] = Item;
    }

    class CItem* GetItem(int Index)
    {
        return m_Item[Index];
    }

    void RemoveItem(int Index);

    void OutputInventory();

private:
    int Menu(class CPlayer* Player);

public:
    bool Init();
    void Run();

private:
    static CInventory* m_pInst;

public:
    static CInventory* GetInst()
    {
        if (m_pInst == nullptr)
            m_pInst = new CInventory;

        return m_pInst;
    }

    static void DestroyInst()
    {
        if (m_pInst)
        {
            delete  m_pInst;
            m_pInst = nullptr;
        }
    }
};

