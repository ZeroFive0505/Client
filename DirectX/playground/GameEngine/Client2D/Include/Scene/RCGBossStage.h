#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/RCGMainWidget.h"
#include "../Widget/RCGBossWidget.h"
#include "../Object/BossEventTrigger.h"
#include "../Object/TileMap.h"

class CRCGBossStage :
	public CSceneMode
{
public:
	CRCGBossStage();
	virtual ~CRCGBossStage();

private:
	CSharedPtr<CTileMap> m_TileMap;
	CSharedPtr<CRCGMainWidget> m_RCGMainWidget;
	CSharedPtr<CBossEventTrigger> m_Trigger;
	std::vector<int> m_vecPilarIndex;
	CSharedPtr<class CRCGPlayer> m_Player;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);

private:
	void CreateMaterial();
	void CreateParticle();

public:
	void ReloadScene(float deltaTime);
};

