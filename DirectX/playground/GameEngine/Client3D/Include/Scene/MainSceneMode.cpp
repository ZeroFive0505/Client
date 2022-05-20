#include "MainSceneMode.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/Viewport.h"
#include "../Object/Player.h"
#include "../Object/LandScapeObj.h"
#include "../Object/Box3D.h"
#include "../Object/Sphere.h"
#include "GameObject/LightObject.h"
#include "Component/LightComponent.h"
#include "Resource/Material/Material.h"
#include "Input.h"

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

	// CBox3D* box = m_Scene->CreateGameObject<CBox3D>("Sphere");

	// box->SetWorldPos(0.0f, 0.0f, 50.0f);

	// box->SetWorldRotationY(45.0f);

	if (m_LoadingFunction)
		m_LoadingFunction(true, 1.0f);

	CLandScapeObj* landScape = m_Scene->CreateGameObject<CLandScapeObj>("LandScape");

	landScape->SetPlayer(player);

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

	m_Scene->GetSceneResource()->LoadMesh(Mesh_Type::Static, "Blade", TEXT("Blade.fbx"));

	m_Scene->GetSceneResource()->LoadSkeleton("PlayerSkeleton", TEXT("Player_Default.bne"), MESH_PATH);

	m_Scene->GetSceneResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");

	m_Scene->GetSceneResource()->AddSocket("PlayerSkeleton", "bone11", "Weapon");

	m_Scene->GetSceneResource()->LoadAnimationSequence(true, "PlayerIdle", TEXT("Player_Default.sqc"), MESH_PATH);

	m_Scene->GetSceneResource()->LoadAnimationSequence(false, "PlayerAttack", TEXT("PlayerAttack.sqc"), MESH_PATH);

	m_Scene->GetSceneResource()->LoadAnimationSequence(true, "PlayerWalk", TEXT("sqc_Battle_Walk.sqc"), MESH_PATH);
}

void CMainSceneMode::CreateMaterial()
{
	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("LandScape");

	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("LandScape");


	material->AddTexture(0, (int)Buffer_Shader_Type::Pixel,
		"LandScapeSplat1Dif", TEXT("LandScape/ROCK_01+MOSS_COLOR_1.png"));
	material->AddTexture(1, (int)Buffer_Shader_Type::Pixel,
		"LandScapeSplat1Nrm", TEXT("LandScape/ROCK_01+MOSS_NRM.png"));
	material->AddTexture(2, (int)Buffer_Shader_Type::Pixel,
		"LandScapeSplat1Spc", TEXT("LandScape/ROCK_01+MOSS_SPEC.png"));

	std::vector<TCHAR*> vecDiffuseFileName;
	std::vector<TCHAR*> vecNormalFileName;
	std::vector<TCHAR*> vecSpecularFileName;
	std::vector<TCHAR*> vecAlphaFileName;

	TCHAR* fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/RoadAlpha1.bmp"));
	vecAlphaFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/GrassFirstAlpha.bmp"));
	vecAlphaFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/SandBaseAlpha.bmp"));
	vecAlphaFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/WaterBaseAlpha.bmp"));
	vecAlphaFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecDiffuseFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecNormalFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/BD_Terrain_Cave_01_SPEC.bmp"));
	vecSpecularFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecDiffuseFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecNormalFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecSpecularFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/Terrain_Cliff_15_Large.dds"));
	vecDiffuseFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));
	vecNormalFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));
	vecSpecularFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecDiffuseFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecNormalFileName.push_back(fileName);

	fileName = new TCHAR[MAX_PATH];
	memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(fileName, TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecSpecularFileName.push_back(fileName);

	material->AddTextureArray(30, (int)Buffer_Shader_Type::Pixel, "SplatDiffuse", vecDiffuseFileName);
	material->AddTextureArray(31, (int)Buffer_Shader_Type::Pixel, "SplatNormal", vecNormalFileName);
	material->AddTextureArray(32, (int)Buffer_Shader_Type::Pixel, "SplatSpecular", vecSpecularFileName);
	material->AddTextureArray(33, (int)Buffer_Shader_Type::Pixel, "SplatAlpha", vecAlphaFileName);

	for (auto i : vecDiffuseFileName)
	{
		SAFE_DELETE_ARRAY(i);
	}

	for (auto i : vecNormalFileName)
	{
		SAFE_DELETE_ARRAY(i);
	}

	for (auto i : vecSpecularFileName)
	{
		SAFE_DELETE_ARRAY(i);
	}

	for (auto i : vecAlphaFileName)
	{
		SAFE_DELETE_ARRAY(i);
	}

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
