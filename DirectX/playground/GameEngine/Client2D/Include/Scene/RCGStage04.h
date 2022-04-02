#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/RCGMainWidget.h"
#include "../Object/TileMap.h"

class CRCGStage04 :
	public CSceneMode
{
public:
	CRCGStage04();
	virtual ~CRCGStage04();

private:
	CSharedPtr<CTileMap> m_TileMap;
	CSharedPtr<CRCGMainWidget> m_RCGMainWidget;
	CSharedPtr<class CRCGPlayer> m_Player;

public:
	virtual bool Init();
	virtual void Start();

private:
	void CreateMaterial();
	void CreateParticle();

public:
	void SceneChange();
	void ReloadScene(float deltaTime);
};

