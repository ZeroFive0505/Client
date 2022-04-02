#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/MainWidget.h"
#include "../Widget/RCGMainWidget.h"
#include "../Object/TileMap.h"

class CMainScene :
	public CSceneMode
{
public:
	CMainScene();
	virtual ~CMainScene();

private:
	CSharedPtr<CMainWidget> m_MainWidget;
	CSharedPtr<CTileMap> m_TileMap;
	CSharedPtr<CRCGMainWidget> m_RCGMainWidget;
	// 스레드 전용 콜백
	std::function<void(bool, float)> m_LoadingFunction;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);

private:
	void CreateMaterial();
	void CreateAnimationSequence();
	void CreateParticle();

public:
	template <typename T>
	void SetLoadingFunction(T* obj, void(T::* func)(bool, float))
	{
		m_LoadingFunction = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
};

