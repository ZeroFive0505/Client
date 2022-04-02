#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/RCGLoadingWidget.h"

class CRCGLoadingScene :
	public CSceneMode
{
public:
	CRCGLoadingScene();
	~CRCGLoadingScene();

private:
	CSharedPtr<CRCGLoadingWidget> m_LoadingWidget;
	class CLoadingThread* m_LoadingThread;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

private:
	void LoadHitEffect();
};

