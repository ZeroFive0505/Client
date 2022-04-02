#pragma once

// �������� ���� ��� ����.
#include "Game.h"

// �� �Ŵ����� ���� �����ϴ� �̱���
class CMapManager
{
private:
	CMapManager();
	~CMapManager();

private:
	// ���漱���� �̷������� ����� ���ÿ��� �����ϴ�.
	class CMap* m_pMap[MT_End];

private:
	// ���� �Ŵ����� ���������� �ڱ⸸�� �޴��� �ҷ��� ���� �޼ҵ�
	Map_Menu Menu();

public:
	bool Init();
	void Run();

private:
	static CMapManager* m_pInst;

public:
	static CMapManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CMapManager;

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

