#include "GameManager.h"


int main()
{
	// ���� ��� ������ �ʱ�ȭ�� �����ߴٸ� �̱��� �ı� �� �����Ѵ�.
	if (!CGameManager::GetInst()->Init())
	{
		CGameManager::DestroyInst();
		return 0;
	}

	// Run�� ���� �ݺ����̴�. Run���� Exit�� ����� ��� ���� �ڵ�� ����.
	CGameManager::GetInst()->Run();

	CGameManager::DestroyInst();

	return 0;
}