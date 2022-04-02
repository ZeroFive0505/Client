#pragma once

#include "Gameinfo.h"
#include "RCGGameManager.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

	void CreateSceneMode(class CScene* scene, size_t type);
	class CGameObject* CreateObject(class CScene* scene, size_t type);
	class CComponent* CreateComponent(class CGameObject* obj, size_t type);

private:
	CRCGGameManager* m_Manager;

public:
	inline CRCGGameManager* GetManager() const
	{
		return m_Manager;
	}

	DECLARE_SINGLE(CClientManager)
};

