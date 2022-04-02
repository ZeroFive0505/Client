#pragma once

#include "Game.h"

class CStoreManager
{
private:
    CStoreManager();
    ~CStoreManager();

private:
    class CStore* m_Store[ST_End];

private:
    Store_Menu Menu();

public:
    bool Init();
    void Run();
    // 몬스터 이름으로 탐색해서 이 몬스터를 이용해서 복제된 새로운 몬스터를
    // 생성할 수 있게 한다.

private:
    static CStoreManager* m_pInst;

public:
    static CStoreManager* GetInst()
    {
        if (m_pInst == nullptr)
            m_pInst = new CStoreManager;

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

