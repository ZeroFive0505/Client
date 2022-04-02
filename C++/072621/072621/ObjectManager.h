#pragma once

// ������Ʈ �Ŵ����� ���������� Game.h ����
#include "Game.h"

// ������Ʈ �Ŵ����� �̱����̴�.
class CObjectManager
{
private:
	CObjectManager();
	~CObjectManager();

private:
	// ������Ʈ �Ŵ����� ������Ʈ���� �����ϱ⿡ ��������� �����Ѵ�. �� �Ŵ����� ����
	class CPlayer* m_pPlayer;
	class CMonster* m_pMonster[3];

public:
	bool Init();
	// ���� �̸����� Ž���ؼ� �� ���͸� �̿��� ������ ���ο� ���͸� �����ϰ� ��ȯ�Ѵ�.
	class CMonster* CloneMonster(const char* pName);

private:
	static CObjectManager* m_pInst;

public:
	static CObjectManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CObjectManager;

		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = nullptr;
		}
	}
};

