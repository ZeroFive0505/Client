#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/RCGMainWidget.h"
#include "../Object/TileMap.h"

class CRCGStage01 :
	public CSceneMode
{
public:
	CRCGStage01();
	virtual ~CRCGStage01();

private:
	CSharedPtr<CTileMap> m_TileMap;
	CSharedPtr<CRCGMainWidget> m_RCGMainWidget;
	std::function<void(bool, float)> m_LoadingFunction;
	CSharedPtr<class CRCGPlayer> m_Player;
	CSharedPtr<class CRCGEnemy> m_DummyEnemy;

	bool m_SpawnEnemies;
	int m_EnemiesToSpawn;
	int m_SpawnCnt;
	float m_ElapsedTime;
	float m_TimeToSpawn;

	bool m_AIStart;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void Start();

private:
	void CreateMaterial();
	void CreateParticle();

public:
	void SceneChange();
	void ColliderDebug(float deltaTime);
	void ReloadScene(float deltaTime);
	void AIStart(float deltaTime);
	void SpawnStart(float deltaTime);

	template <typename T>
	void SetLoadingFunction(T* obj, void(T::* func)(bool, float))
	{
		m_LoadingFunction = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
};

