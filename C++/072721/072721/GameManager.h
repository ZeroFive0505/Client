#pragma once

#include "Game.h"

class CGameManager
{
private:
    CGameManager();
    ~CGameManager();

private:
    Main_Menu Menu();

public:
    bool Init();
    void Run();

private:
    static CGameManager* m_pInst;

public:
    static CGameManager* GetInst()
    {
        if (m_pInst == nullptr)
            m_pInst = new CGameManager;

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

