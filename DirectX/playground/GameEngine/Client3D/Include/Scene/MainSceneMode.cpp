#include "MainSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/Viewport.h"
#include "../Object/Player.h"
#include "../Object/LandScapeObj.h"
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

	CLandScapeObj* landScape = m_Scene->CreateGameObject<CLandScapeObj>("LandScape");

	CLightObject* Light = m_Scene->CreateGameObject<CLightObject>("Light1");

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
	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("LandScape");

	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("LandScape");


	material->AddTexture(0, (int)Buffer_Shader_Type::Pixel,
		"LandScapeSplat1Dif", TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	material->AddTexture(1, (int)Buffer_Shader_Type::Pixel,
		"LandScapeSplat1Nrm", TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	material->AddTexture(2, (int)Buffer_Shader_Type::Pixel,
		"LandScapeSplat1Spc", TEXT("LandScape/BD_Terrain_Cave_01_SPEC.bmp"));
	material->SetShader("LandScapeShader");

	material->EnableBump();
	material->EnableSpecularTex();
	material->SetSpecularPower(2.0f);
}

void CMainSceneMode::CreateAnimationSequence()
{
}

void CMainSceneMode::CreateParticle()
{
}
