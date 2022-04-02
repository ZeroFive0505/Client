#pragma once

#include "../GameInfo.h"

/// <summary>
/// CollisionManager 클래스는 충돌 프로파일을 관리하는 클래스이다.
/// 이곳에서 충돌체를 조회해서 충돌 검사를 한다.
/// </summary>
class CCollisionManager
{
private:
	CCollisionManager();
	~CCollisionManager();

private:
	// 충돌체의 이름과 프로파일
	std::unordered_map<std::string, CollisionProfile*> m_mapProfile;

public:
	bool Init();
	bool CreateProfile(const std::string& Name, ECollision_Channel Channel,
		bool Enable, ECollision_State State = ECollision_State::Collision);
	bool SetCollisionState(const std::string& Name, ECollision_Channel Channel,
		ECollision_State State);
	CollisionProfile* FindProfile(const std::string& Name);

private:
	static CCollisionManager* m_Inst;

public:
	static CCollisionManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CCollisionManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

