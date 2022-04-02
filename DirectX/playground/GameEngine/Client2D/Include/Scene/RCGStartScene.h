#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/RCGStartWidget.h"

class CRCGStartScene :
	public CSceneMode
{
public:
	CRCGStartScene();
	~CRCGStartScene();

private:
	CSharedPtr<CRCGStartWidget> m_StartWidget;

public:
	virtual bool Init();
};