#include "MainSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/Viewport.h"
#include "../Object/Player.h"
#include "GameObject/LightObject.h"
#include "Component/LightComponent.h"
#include "Resource/Material/Material.h"

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


	//CLightObject* Light = m_Scene->CreateGameObject<CLightObject>("Light1");

	//((CLightComponent*)Light->GetRootComponent())->SetRelativePos(-3.f, 5.f, 0.f);
	//((CLightComponent*)Light->GetRootComponent())->SetLightType(Light_Type::Point);
	//((CLightComponent*)Light->GetRootComponent())->SetDistance(10.f);
	//((CLightComponent*)Light->GetRootComponent())->SetAttConst3(0.02f);
	//((CLightComponent*)Light->GetRootComponent())->SetColor(Vector4(1.f, 0.f, 0.f, 1.f));

	//CLightObject* Light2 = m_Scene->CreateGameObject<CLightObject>("Light2");

	//((CLightComponent*)Light2->GetRootComponent())->SetRelativePos(3.f, 5.f, 0.f);
	//((CLightComponent*)Light2->GetRootComponent())->SetLightType(Light_Type::Point);
	//((CLightComponent*)Light2->GetRootComponent())->SetDistance(10.f);
	//((CLightComponent*)Light2->GetRootComponent())->SetAttConst3(0.02f);
	//((CLightComponent*)Light2->GetRootComponent())->SetColor(Vector4(0.f, 1.f, 0.f, 1.f));

	return true;
}

void CMainSceneMode::LoadMesh()
{
	m_Scene->GetSceneResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh", TEXT("Player_Default.msh"));

	m_Scene->GetSceneResource()->LoadSkeleton("PlayerSkeleton", TEXT("Player_Default.bne"), MESH_PATH);

	m_Scene->GetSceneResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	m_Scene->GetSceneResource()->LoadAnimationSequence(true, "PlayerIdle", TEXT("Player_Default.sqc"), MESH_PATH);

	m_Scene->GetSceneResource()->LoadAnimationSequence(false, "PlayerAttack", TEXT("PlayerAttack.sqc"), MESH_PATH);

	m_Scene->GetSceneResource()->LoadAnimationSequence(true, "PlayerWalk", TEXT("sqc_Battle_Walk.sqc"), MESH_PATH);
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
