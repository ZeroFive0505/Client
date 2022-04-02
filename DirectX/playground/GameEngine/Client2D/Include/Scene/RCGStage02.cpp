#include "RCGStage02.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/RCGPlayer.h"
#include "../Object/Kyoko.h"
#include "../Object/SchoolBoy.h"
#include "../Object/SchoolGirl.h"
#include "../Object/Cheerleader.h"
#include "../Object/MT.h"
#include "../Object/RCGDoor.h"
#include "../Object/CameraPoint.h"
#include "RCGStage03.h"
#include "Input.h"

CRCGStage02::CRCGStage02()
{
    SetTypeID<CRCGStage02>();
}

CRCGStage02::~CRCGStage02()
{
}

bool CRCGStage02::Init()
{
    CreateMaterial();

    CreateParticle();

	m_Player = m_Scene->CreateGameObject<CKyoko>("Kyoko");

	CCameraPoint* cam = m_Scene->CreateGameObject<CCameraPoint>("Cam");

	SetPlayerObject(m_Player);

	m_Player->SetCameraPoint(cam);

	cam->SetTarget(m_Player);

	cam->StartFollow();

	CRCGDoor* door = m_Scene->CreateGameObject<CRCGDoor>("Door");

	m_TileMap = m_Scene->LoadGameObject<CTileMap>();

	m_TileMap->Load("Stage2.gobj", SCENE_PATH);

	m_TileMap->SetTileMap();

	m_TileMap->GetTileMap()->Start();

	m_TileMap->SetWorldPos(0.0f, 0.0f, 999.0f);

	door->SetWorldPos(1518.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 550.0f, 0.0f);

	m_Player->SetWorldPos(350.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 540.0f, 0.0f);

	CSchoolGirl* schoolGirl = m_Scene->CreateGameObject<CSchoolGirl>("Enemy 1");

	schoolGirl->SetIdleState();

	schoolGirl->SetWorldPos(700.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 440.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(schoolGirl);

	schoolGirl = m_Scene->CreateGameObject<CSchoolGirl>("Enemy 2");

	schoolGirl->SetIdleState();

	schoolGirl->SetWorldPos(600.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 500.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(schoolGirl);

	schoolGirl = m_Scene->CreateGameObject<CSchoolGirl>("Enemy 3");

	schoolGirl->SetIdleState();

	schoolGirl->SetWorldPos(800.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 540.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(schoolGirl);

	schoolGirl = m_Scene->CreateGameObject<CSchoolGirl>("Enemy 4");

	schoolGirl->SetIdleState();

	schoolGirl->SetWorldPos(1200.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 540.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(schoolGirl);

	schoolGirl = m_Scene->CreateGameObject<CSchoolGirl>("Enemy 5");

	schoolGirl->SetIdleState();

	schoolGirl->SetWorldPos(1350.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 600.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(schoolGirl);

	CClientManager::GetInst()->GetManager()->AddSpawnPoint(door->GetWorldPos());
	CClientManager::GetInst()->GetManager()->AddSpawnPoint(m_Player->GetWorldPos());

	m_RCGMainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGMainWidget>("MainWidget");

	m_Player->SetMainWidget(m_RCGMainWidget);

	m_RCGMainWidget->DisableBattleIntro();

	m_RCGMainWidget->SetSceneChangeCallback<CRCGStage02>(this, &CRCGStage02::SceneChange);

	m_Player->SetHP();

	m_Player->SetSP();

	door->SetWidget(m_RCGMainWidget);

	CInput::GetInst()->SetCallback<CRCGStage02>("Enter", KeyState_Down, this, &CRCGStage02::ReloadScene);

    return true;
}

void CRCGStage02::Start()
{
	int countX = m_TileMap->GetTileMap()->GetTileCountX();
	int countY = m_TileMap->GetTileMap()->GetTileCountY();

	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j;

			if (m_TileMap->GetTileMap()->GetTile(index)->GetTileType() == Tile_Type::Wall)
			{
				CGameObject* tile = m_Scene->CreateGameObject<CGameObject>("Wall " + std::to_string(index + 1));
				tile->SetTypeID<CTile>();
				CColliderBox2D* collider = tile->CreateComponent<CColliderBox2D>("Wall Collider " + std::to_string(index + 1));
				tile->SetPivot(0.5f, 0.5f, 0.0f);
				tile->SetWorldPos(m_TileMap->GetTileMap()->GetTile(index)->GetCenter());
				collider->SetCollisionProfile("Wall");
				m_Scene->AddTileColldier((CColliderComponent*)collider);

				Vector3 colliderSize;

				colliderSize = m_TileMap->GetTileMap()->GetTileSize();

				collider->SetExtent(colliderSize.x / 2.0f, colliderSize.y / 2.0f);
			}
			else if (m_TileMap->GetTileMap()->GetTile(index)->GetTileType() == Tile_Type::SideWall)
			{
				CGameObject* tile = m_Scene->CreateGameObject<CGameObject>("Wall " + std::to_string(index + 1));
				tile->SetTypeID<CTile>();
				CColliderBox2D* collider = tile->CreateComponent<CColliderBox2D>("Wall Collider " + std::to_string(index + 1));
				tile->SetPivot(0.5f, 0.5f, 0.0f);
				tile->SetWorldPos(m_TileMap->GetTileMap()->GetTile(index)->GetCenter());
				collider->SetCollisionProfile("SideWall");
				m_Scene->AddTileColldier((CColliderComponent*)collider);

				Vector3 colliderSize;

				colliderSize = m_TileMap->GetTileMap()->GetTileSize();

				collider->SetExtent(colliderSize.x / 2.0f, colliderSize.y / 2.0f);
			}
		}
	}
}


void CRCGStage02::CreateMaterial()
{
	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("Bubble");
	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("Bubble");

	material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));

	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");

	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("SmokeParticle");
	material = m_Scene->GetSceneResource()->FindMaterial("SmokeParticle");

	material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "SmokeParticle", TEXT("Particle/fx_smoke_thick.png"));

	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");

	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("TileMap");
	material = m_Scene->GetSceneResource()->FindMaterial("TileMap");

	material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Tile", TEXT("Floors.png"));

	material->SetShader("TileMapShader");
	material->SetRenderState("AlphaBlend");

	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("DiabloTileMap");
	material = m_Scene->GetSceneResource()->FindMaterial("DiabloTileMap");

	material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "DiabloTile", TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

	material->SetShader("TileMapShader");
	material->SetRenderState("AlphaBlend");
}

void CRCGStage02::CreateParticle()
{
	m_Scene->GetSceneResource()->CreateParticle("Bubble");
	CParticle* particle = m_Scene->GetSceneResource()->FindParticle("Bubble");
	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("Bubble");

	particle->SetMaterial(material);
	particle->SetSpawnCountMax(500);
	particle->SetLifeTimeMin(1.0f);
	particle->SetLifeTimeMax(2.0f);
	particle->SetScaleMin(Vector3(20.0f, 20.0f, 1.0f));
	particle->SetScaleMax(Vector3(50.0f, 50.0f, 1.0f));
	particle->SetSpeedMin(100.0f);
	particle->SetSpeedMax(300.0f);
	particle->SetMoveDir(Vector3(0.0f, 1.0f, 0.0f));
	particle->SetMoveAngle(Vector3(0.0f, 0.0f, 60.0f));
	particle->SetStartMin(Vector3(-30.0f, -30.0f, 0.0f));
	particle->SetStartMax(Vector3(30.0f, 30.0f, 0.0f));
	particle->SetMove(true);



	m_Scene->GetSceneResource()->CreateParticle("SmokeParticle");
	particle = m_Scene->GetSceneResource()->FindParticle("SmokeParticle");
	material = m_Scene->GetSceneResource()->FindMaterial("SmokeParticle");

	particle->SetMaterial(material);
	particle->SetSpawnCountMax(50);
	particle->SetLifeTimeMin(0.2f);
	particle->SetLifeTimeMax(0.5f);
	particle->SetScaleMin(Vector3(32.0, 32.0f, 1.0f));
	particle->SetScaleMax(Vector3(64.0f, 64.0f, 1.0f));
	particle->SetStartMin(Vector3(-50.0f, -50.0f, 0.0f));
	particle->SetStartMax(Vector3(50.0f, 50.0f, 0.0f));
	particle->SetMoveDir(Vector3(0.0f, 1.0f, 0.0f));
	particle->SetMoveAngle(Vector3(0.0f, 0.0f, 45.0f));
	particle->SetColorMin(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	particle->SetColorMax(Vector4(1.0f, 1.0f, 1.0f, 0.7f));
	particle->SetSpeedMin(50.0f);
	particle->SetSpeedMax(100.0f);
	particle->SetMove(true);
}

void CRCGStage02::SceneChange()
{
	CClientManager::GetInst()->GetManager()->ClearEnemies();
	CClientManager::GetInst()->GetManager()->ClearSpawnPoint();
	CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CRCGStage03>(false);
}

void CRCGStage02::ReloadScene(float deltaTime)
{
	if (m_Player->GetHealth() <= 0 && !CEngine::GetInst()->IsPlay())
	{
		CClientManager::GetInst()->GetManager()->ClearEnemies();
		CClientManager::GetInst()->GetManager()->ClearSpawnPoint();
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneMode<CRCGStage02>(false);
		CEngine::GetInst()->SetPlay(true);
	}
}
