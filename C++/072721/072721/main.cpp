
#include "GameManager.h"

int main()
{
    if (!CGameManager::GetInst()->Init())
    {
        CGameManager::DestroyInst();
        return 0;
    }

    CGameManager::GetInst()->Run();

    CGameManager::DestroyInst();

    return 0;
}
