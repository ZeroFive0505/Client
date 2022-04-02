#include "SceneMode.h"
#include "../GameObject/GameObject.h"

CSceneMode::CSceneMode()
{
    SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::SetPlayerObject(CGameObject* obj)
{
    m_PlayerObject = obj;
}

void CSceneMode::Start()
{
}

bool CSceneMode::Init()
{
    return true;
}

void CSceneMode::Update(float deltaTime)
{
}

void CSceneMode::PostUpdate(float deltaTime)
{
}
