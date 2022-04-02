#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneResource.h"
#include "../Object/Enemy.h"
#include "../Object/PixelTest.h"
#include "../Object/Observer.h"
#include "../Object/BubbleParticle.h"
#include "../Object/CameraPoint.h"

CMainScene::CMainScene()
{
	SetTypeID<CMainScene>();
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	// 로딩 함수의 스레드큐에 메시지를 추가한다.
	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.3f);

	CPlayer2D* player = m_Scene->CreateGameObject<CPlayer2D>("Player2D");

	CCameraPoint* cam = m_Scene->CreateGameObject<CCameraPoint>("Follow Cam");

	cam->SetTarget(player);

	SetPlayerObject(player);

	m_TileMap = m_Scene->LoadGameObject<CTileMap>();

	m_TileMap->Load("TestTile.gobj", SCENE_PATH);

	m_TileMap->SetTileMap();

	m_TileMap->GetTileMap()->Start();

	m_TileMap->SetWorldPos(0.0f, 0.0f, 999.0f);

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.9f);

	// CPixelTest* pixelTest = m_Scene->CreateGameObject<CPixelTest>("PixelText");

	// CBubbleParticle* particle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	// particle->SetRelativePos(-200.0f, 0.0f, 0.0f);

	m_MainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CMainWidget>("MainWidget");
	// m_RCGMainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGMainWidget>("MainWidget");
	// player->SetMainWidget(m_RCGMainWidget);

	return true;
}

void CMainScene::Start()
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

void CMainScene::Update(float deltaTime)
{
}

void CMainScene::CreateMaterial()
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

void CMainScene::CreateAnimationSequence()
{
	//// PlayerIdle이라는 시퀀스는 Player 텍스쳐에 있다.
	//m_Scene->GetSceneResource()->CreateAnimationSequence2D("PlayerIdle", "Player", TEXT("Test.png"));
	//m_Scene->GetSceneResource()->CreateAnimationSequence2D("PlayerWalk", "Player", TEXT("Test.png"));
	//


	//for (int i = 0; i < 11; i++)
	//{
	//	// PlayerIdle시퀀스에 애니메이션을 추가한다.
	//	m_Scene->GetSceneResource()->AddAnimationSequence2DFrame("PlayerIdle", Vector2(i * 38.2f, 0.0f), Vector2(38.2f, 73.0f));
	//}

	//for (int i = 12; i < 24; i++)
	//{
	//	// PlayerWalk시퀀스에 애니메이션을 추가한다.
	//	m_Scene->GetSceneResource()->AddAnimationSequence2DFrame("PlayerWalk", Vector2(i * 38.2f, 0.0f), Vector2(38.2f, 73.0f));
	//}

	m_Scene->GetSceneResource()->CreateAnimationSequence2D("EnemyIdle", "Enemy", TEXT("Player.png"));



	for (int i = 0; i < 7; ++i)
	{
		m_Scene->GetSceneResource()->AddAnimationSequence2DFrame("EnemyIdle", Vector2(i * 50.f, 148.f), Vector2(50.f, 37.f));
	}
}

void CMainScene::CreateParticle()
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
