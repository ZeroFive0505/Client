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
    // ���� �̸����� Ž���ؼ� �� ���͸� �̿��ؼ� ������ ���ο� ���͸�
    // ������ �� �ְ� �Ѵ�.

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

