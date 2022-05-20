#pragma once

#include "GameInfo.h"

class CClientManager3D
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

	void CreateSceneMode(class CScene* scene, size_t type);
	class CGameObject* CreateObject(class CScene* scene, size_t type);
	class CComponent* CreateComponent(class CGameObject* obj, size_t type);

private:
	void MousePick(float deltaTime);

	DECLARE_SINGLE(CClientManager3D)
};

