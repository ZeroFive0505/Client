#include "RCGBossStage.h"
#include "../Object/Kyoko.h"
#include "../Object/CameraPoint.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/BossEventTrigger.h"
#include "../Object/RCGPilar.h"
#include "Input.h"

CRCGBossStage::CRCGBossStage()
{
    SetTypeID<CRCGBossStage>();
}

CRCGBossStage::~CRCGBossStage()
{
}

bool CRCGBossStage::Init()
{
    CreateMaterial();

    CreateParticle();

	m_Player = m_Scene->CreateGameObject<CKyoko>("Kyoko");

    SetPlayerObject(m_Player);

    CCameraPoint* camera = m_Scene->CreateGameObject<CCameraPoint>("Cam");

	m_Player->SetCameraPoint(camera);

    camera->SetTarget(m_Player);

    camera->StartFollow();

    m_TileMap = m_Scene->LoadGameObject<CTileMap>();

    m_TileMap->Load("BossStage.gobj", SCENE_PATH);

    m_TileMap->SetTileMap();

    m_TileMap->GetTileMap()->Start();

    m_TileMap->SetWorldPos(0.0f, 0.0f, 999.0f);

	m_Player->SetWorldPos(490.0f, (float)m_TileMap->GetTileMap()->GetBackMaterial()->GetTextureHeight() * 2.5f - 665.0f, 0.0f);

	m_RCGMainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGMainWidget>("MainWidget");

	m_Player->SetMainWidget(m_RCGMainWidget);

	m_RCGMainWidget->DisableBattleIntro();

	m_Trigger = m_Scene->CreateGameObject<CBossEventTrigger>("Boss Trigger");

	m_Trigger->SetWorldPos(m_TileMap->GetWorldScale().x * 0.35f, m_TileMap->GetWorldScale().y * 0.5f, 0.0f);

	m_Trigger->SetTriggerExtent(100.0f, m_TileMap->GetWorldScale().y);

	m_Player->SetHP();

	m_Player->SetSP();

    return true;
}

void CRCGBossStage::Start()
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
			else if (m_TileMap->GetTileMap()->GetTile(index)->GetTileType() == Tile_Type::Pilar)
				m_vecPilarIndex.push_back(index);
		}
	}


	size_t size = m_vecPilarIndex.size();

	for (size_t i = 0; i < size; i++)
	{
		int index = m_vecPilarIndex[i];

		CTile* tile = m_TileMap->GetTileMap()->GetTile(index);

		std::string name = "Pilar " + std::to_string(i + 1);

		CRCGPilar* pilar = m_Scene->CreateGameObject<CRCGPilar>(name);

		TCHAR fileName[MAX_PATH] = {};

		wsprintf(fileName, TEXT("lobby_l3_pillar_%d.png"), (int)(i + 1));

		pilar->SetPilarTexture(name.c_str(), fileName);

		pilar->SetWorldPos(tile->GetCenter());

		pilar->SetTileIndex(index);

		m_Trigger->AddPilar(pilar);
		pilar->SetOwnerTrigger(m_Trigger);
	}
}

void CRCGBossStage::Update(float deltaTime)
{
}

void CRCGBossStage::CreateMaterial()
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

void CRCGBossStage::CreateParticle()
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

void CRCGBossStage::ReloadScene(float deltaTime)
{
	if (m_Player->GetHealth() <= 0 && !CEngine::GetInst()->IsPlay())
	{
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneMode<CRCGBossStage>(false);
		CEngine::GetInst()->SetPlay(true);
	}
}
