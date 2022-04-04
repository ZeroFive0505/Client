#include "RCGStage01.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Scene/RCGStage02.h"
#include "../Scene/RCGStage03.h"
#include "../Scene/RCGStage04.h"
#include "../Scene/RCGBossStage.h"
#include "../Object/RCGPlayer.h"
#include "../Object/Kyoko.h"
#include "../Object/SchoolBoy.h"
#include "../Object/SchoolGirl.h"
#include "../Object/Cheerleader.h"
#include "../Object/MT.h"
#include "../Object/RCGDoor.h"
#include "../Object/LockEventTrigger.h"
#include "../Object/CameraPoint.h"
#include "Input.h"

CRCGStage01::CRCGStage01()
{
    SetTypeID<CRCGStage01>();
	m_ElapsedTime = 0.0f;
	m_SpawnEnemies = false;
	m_AIStart = false;
}

CRCGStage01::~CRCGStage01()
{
}

bool CRCGStage01::Init()
{
    CreateMaterial();

    CreateParticle();

    if (m_LoadingFunction)
        m_LoadingFunction(false, 0.1f);

	m_Player = m_Scene->CreateGameObject<CKyoko>("Kyoko");

	m_Player->SetWorldPos(-100.0f, 100.0f, 0.0f);

	m_Player->SetBodyYPos(200.0f);

	m_Player->BattleStartBegin();

    SetPlayerObject(m_Player);

	CCameraPoint* camera = m_Scene->CreateGameObject<CCameraPoint>("Cam");

	m_Player->SetCameraPoint(camera);

	camera->SetTarget(m_Player);

	camera->StopFollow();

	CResourceManager::GetInst()->SoundPlay("Detention");

	camera->SetWorldPos(300.0f, 150.0f, 0.0f);

    CRCGDoor* door = m_Scene->CreateGameObject<CRCGDoor>("Door");

    door->SetWorldPos(1195.0f, 309.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddSpawnPoint(door->GetWorldPos());

	m_DummyEnemy = m_Scene->CreateGameObject<CSchoolBoy>("Enemy 1");

	m_DummyEnemy->SetHealth(9999);

	m_DummyEnemy->SetWorldPos(900.0f, 120.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(m_DummyEnemy);

    if (m_LoadingFunction)
        m_LoadingFunction(false, 0.4f);

    m_TileMap = m_Scene->LoadGameObject<CTileMap>();

    m_TileMap->Load("Stage1.gobj", SCENE_PATH);

    m_TileMap->SetTileMap();

    m_TileMap->GetTileMap()->Start();

    m_TileMap->SetWorldPos(0.0f, 0.0f, 999.0f);

    if (m_LoadingFunction)
        m_LoadingFunction(false, 0.9f);

    m_RCGMainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGMainWidget>("MainWidget");

	m_Player->SetMainWidget(m_RCGMainWidget);

	m_RCGMainWidget->DisableBattleIntro();

	m_RCGMainWidget->SetSceneChangeCallback<CRCGStage01>(this, &CRCGStage01::SceneChange);

	door->SetWidget(m_RCGMainWidget);

	CInput::GetInst()->SetCallback<CRCGStage01>("Enter", KeyState_Down, this, &CRCGStage01::ReloadScene);
	CInput::GetInst()->SetCallback<CRCGStage01>("Play", KeyState_Down, this, &CRCGStage01::SpawnStart);
	CInput::GetInst()->SetCallback<CRCGStage01>("Debug", KeyState_Down, this, &CRCGStage01::ColliderDebug);
	CInput::GetInst()->SetCallback<CRCGStage01>("AI", KeyState_Down, this, &CRCGStage01::AIStart);

    return true;
}

void CRCGStage01::Update(float deltaTime)
{
	if (m_SpawnEnemies)
	{
		if (CClientManager::GetInst()->GetManager()->CanSpawnEnemy())
		{
			m_ElapsedTime += deltaTime;

			if (m_EnemiesToSpawn > m_SpawnCnt && m_ElapsedTime >= m_TimeToSpawn)
			{
				CRCGEnemy* enemy;		
				enemy = m_Scene->CreateGameObject<CSchoolBoy>("SchoolBoy " + std::to_string(m_SpawnCnt + 1));
				((CSchoolBoy*)enemy)->SetPursueState();
				Vector3 spawnPos = CClientManager::GetInst()->GetManager()->GetRandomSpanwPoint();
				enemy->SetWorldPos(spawnPos);
				enemy->StartSpawn();
				CClientManager::GetInst()->GetManager()->AddEnemy(enemy);

				m_SpawnCnt++;
				m_ElapsedTime = 0.0f;
			}
			else if (m_SpawnCnt >= m_EnemiesToSpawn)
				m_SpawnEnemies = false;
		}
	}
}

void CRCGStage01::Start()
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

void CRCGStage01::CreateMaterial()
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

void CRCGStage01::CreateParticle()
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

void CRCGStage01::SceneChange()
{
	CClientManager::GetInst()->GetManager()->ClearEnemies();
	CClientManager::GetInst()->GetManager()->ClearSpawnPoint();
	CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CRCGStage02>(false);
}

void CRCGStage01::ColliderDebug(float deltaTime)
{
	m_Scene->TileCollidersRenderToggle();

	m_Player->CollidersRenderToggle();

	auto enemies = CClientManager::GetInst()->GetManager()->GetEnemies();

	auto iter = enemies.begin();
	auto iterEnd = enemies.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->CollidersRenderToggle();
	}
}

void CRCGStage01::ReloadScene(float deltaTime)
{
	if (m_Player->GetHealth() <= 0 && !CEngine::GetInst()->IsPlay())
	{
		CClientManager::GetInst()->GetManager()->ClearEnemies();
		CClientManager::GetInst()->GetManager()->ClearSpawnPoint();
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneMode<CRCGStage01>(false);
		CEngine::GetInst()->SetPlay(true);
	}
}

void CRCGStage01::AIStart(float deltaTime)
{
	if (m_AIStart)
		return;

	m_DummyEnemy->SetHealth(250);
	((CSchoolBoy*)*m_DummyEnemy)->SetPursueState();
	m_AIStart = true;
}

void CRCGStage01::SpawnStart(float deltaTime)
{
	m_SpawnEnemies = true;
	m_TimeToSpawn = 1.5f;
	m_SpawnCnt = 0;
	m_EnemiesToSpawn = 3;
}
