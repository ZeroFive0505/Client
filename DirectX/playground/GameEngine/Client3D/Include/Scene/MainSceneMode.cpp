#include "MainSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/Viewport.h"
#include "../Object/Player.h"

CMainSceneMode::CMainSceneMode()
{
	SetTypeID<CMainSceneMode>();
}

CMainSceneMode::~CMainSceneMode()
{
}

bool CMainSceneMode::Init()
{
	LoadMesh();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.3f);

	CPlayer* player = m_Scene->CreateGameObject<CPlayer>("Player");

	SetPlayerObject(player);

	if (m_LoadingFunction)
		m_LoadingFunction(true, 1.0f);

	return true;
}

void CMainSceneMode::LoadMesh()
{
	m_Scene->GetSceneResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh", TEXT("Player_Default.msh"));

	m_Scene->GetSceneResource()->LoadSkeleton("PlayerSkeleton", TEXT("Player_Default.bne"), MESH_PATH);

	m_Scene->GetSceneResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	m_Scene->GetSceneResource()->LoadAnimationSequence(true, "PlayerIdle", TEXT("Player_Default.sqc"), MESH_PATH);
}

void CMainSceneMode::CreateMaterial()
{
}

void CMainSceneMode::CreateAnimationSequence()
{
}

void CMainSceneMode::CreateParticle()
{
}
