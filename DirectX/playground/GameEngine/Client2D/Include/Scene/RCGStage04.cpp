#include "RCGStage04.h"
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
#include "RCGBossStage.h"
#include "Input.h"

CRCGStage04::CRCGStage04()
{
    SetTypeID<CRCGStage04>();
}

CRCGStage04::~CRCGStage04()
{
}

bool CRCGStage04::Init()
{
    CreateMaterial();

    CreateParticle();

    m_Player = m_Scene->CreateGameObject<CKyoko>("Kyoko");

    CCameraPoint* cam = m_Scene->CreateGameObject<CCameraPoint>("Cam");

    SetPlayerObject(m_Player);

	m_Player->SetCameraPoint(cam);

    cam->SetTarget(m_Player);

    cam->StartFollow();

    m_TileMap = m_Scene->LoadGameObject<CTileMap>();

    m_TileMap->Load("Stage4.gobj", SCENE_PATH);

    m_TileMap->SetTileMap();

    m_TileMap->GetTileMap()->Start();

    m_TileMap->SetWorldPos(0.0f, 0.0f, 999.0f);

	m_Player->SetWorldPos(130.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 700.0f, 0.0f);

    m_RCGMainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGMainWidget>("MainWidget");

	CRCGDoor* door = m_Scene->CreateGameObject<CRCGDoor>("Door");

	door->SetWorldPos(2240.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 640.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddSpawnPoint(door->GetWorldPos());
	CClientManager::GetInst()->GetManager()->AddSpawnPoint(m_Player->GetWorldPos());

	m_Player->SetMainWidget(m_RCGMainWidget);

	door->SetWidget(m_RCGMainWidget);


	CCheerleader* cheerLeader = m_Scene->CreateGameObject<CCheerleader>("Enemy 1");

	cheerLeader->SetIdleState();

	cheerLeader->SetFacingDirection(false);

	cheerLeader->SetWorldPos(550.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 600.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(cheerLeader);

	/*cheerLeader = m_Scene->CreateGameObject<CCheerleader>("Enemy 2");

	cheerLeader->SetIdleState();

	cheerLeader->SetWorldPos(650.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 650.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(cheerLeader);

	cheerLeader = m_Scene->CreateGameObject<CCheerleader>("Enemy 3");

	cheerLeader->SetIdleState();

	cheerLeader->SetWorldPos(800.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 550.0f, 0.0f);

	CClientManager::GetInst()->GetManager()->AddEnemy(cheerLeader);*/

	m_RCGMainWidget->SetSceneChangeCallback<CRCGStage04>(this, &CRCGStage04::SceneChange);

	CLockEventTrigger* trigger = m_Scene->CreateGameObject<CLockEventTrigger>("Event Trigger");

	trigger->SetWorldPos(m_TileMap->GetWorldScale().x * 0.5f, m_TileMap->GetWorldScale().y * 0.5f, 0.0f);

	trigger->SetTriggerExtent(100.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f);

    m_RCGMainWidget->DisableBattleIntro();

	m_Player->SetHP();

	m_Player->SetSP();

	CInput::GetInst()->SetCallback<CRCGStage04>("Enter", KeyState_Down, this, &CRCGStage04::ReloadScene);

    return true;
}

void CRCGStage04::Start()
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

void CRCGStage04::CreateMaterial()
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

void CRCGStage04::CreateParticle()
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

void CRCGStage04::SceneChange()
{
	CClientManager::GetInst()->GetManager()->ClearEnemies();
	CClientManager::GetInst()->GetManager()->ClearSpawnPoint();
	CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CRCGBossStage>(false);
}

void CRCGStage04::ReloadScene(float deltaTime)
{
	if (m_Player->GetHealth() <= 0 && !CEngine::GetInst()->IsPlay())
	{
		CClientManager::GetInst()->GetManager()->ClearEnemies();
		CClientManager::GetInst()->GetManager()->ClearSpawnPoint();
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneMode<CRCGStage04>(false);
		CEngine::GetInst()->SetPlay(true);
	}
}
