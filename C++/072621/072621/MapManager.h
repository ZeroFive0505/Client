#pragma once

// 공용으로 쓰일 헤더 포함.
#include "Game.h"

// 맵 매니저는 맵을 관리하는 싱글톤
class CMapManager
{
private:
	CMapManager();
	~CMapManager();

private:
	// 전방선언은 이런식으로 선언과 동시에도 가능하다.
	class CMap* m_pMap[MT_End];

private:
	// 게임 매니저와 마찬가지로 자기만의 메뉴를 불러올 개인 메소드
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

