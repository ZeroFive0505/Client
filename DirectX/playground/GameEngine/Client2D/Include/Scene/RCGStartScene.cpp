#include "RCGStartScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"

CRCGStartScene::CRCGStartScene()
{
	SetTypeID<CRCGStartScene>();
}

CRCGStartScene::~CRCGStartScene()
{
}

bool CRCGStartScene::Init()
{
	m_StartWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGStartWidget>("RCGStartWidget");

	return true;
}
