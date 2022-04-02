#include "GameManager.h"


int main()
{
	// 만약 어떠한 이유로 초기화가 실패했다면 싱글톤 파괴 후 종료한다.
	if (!CGameManager::GetInst()->Init())
	{
		CGameManager::DestroyInst();
		return 0;
	}

	// Run은 무한 반복문이다. Run에서 Exit을 골랐을 경우 다음 코드로 진행.
	CGameManager::GetInst()->Run();

	CGameManager::DestroyInst();

	return 0;
}