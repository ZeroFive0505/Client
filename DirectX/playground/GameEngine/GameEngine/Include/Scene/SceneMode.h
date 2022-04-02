#pragma once

#include "../Ref.h"

/// <summary>
/// 씬 모드는 현재 씬의 상태를 나타낸다.
/// 에디터 모드인지, 플레이 모드인지
/// </summary>
class CSceneMode :
	public CRef
{
	friend class CScene;

protected:
	CSceneMode();
	virtual ~CSceneMode();

protected:
	class CScene* m_Scene;
	CSharedPtr<class CGameObject> m_PlayerObject;

public:
	void SetPlayerObject(class CGameObject* obj);

	inline class CGameObject* GetPlayerObject() const
	{
		return m_PlayerObject;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
};

