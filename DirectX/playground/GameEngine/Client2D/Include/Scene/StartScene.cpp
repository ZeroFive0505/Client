#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"

CStartScene::CStartScene()
{
    SetTypeID<CStartScene>();
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
    m_StartWidget = m_Scene->GetViewport()->CreateWidgetWindow<CStartWidget>("StartWidget");

    return true;
}
