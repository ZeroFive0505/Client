#pragma once

// 오브젝트 매니저도 마찬가지로 Game.h 포함
#include "Game.h"

// 오브젝트 매니저도 싱글톤이다.
class CObjectManager
{
private:
	CObjectManager();
	~CObjectManager();

private:
	// 오브젝트 매니저는 오브젝트들을 관리하기에 멤버변수로 소유한다. 맵 매니저도 동일
	class CPlayer* m_pPlayer;
	class CMonster* m_pMonster[3];

public:
	bool Init();
	// 몬스터 이름으로 탐색해서 이 몬스터를 이용해 복제된 새로운 몬스터를 생성하고 반환한다.
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

