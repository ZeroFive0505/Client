#pragma once

// 공용으로 쓰일 변수들과 헤더파일이 포함된 Game.h를 포함.
#include "Game.h"

// 게임매니저는 싱글톤으로 제작
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
			delete m_pInst;
			m_pInst = nullptr;
		}
	}
};

