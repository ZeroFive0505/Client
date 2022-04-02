
#include "MainScene.h"
#include "../Timer.h"
#include "../GameManager.h"
#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "../Scene/Camera.h"
#include "../UI/UIMain.h"
#include "../UI//UICharacterStateHUD.h"
#include "../Map/TileMap.h"
#include "../Map/ScrollMap.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	LoadSound();

	// 초기화시 현재 씬에 필요한 객체와 리소스들을 전부 불러온다.
	LoadAnimationSequence();

	GetCamera()->SetWorldResolution(1920.0f, 1080.0f);

	// 씬 초기화시 프로토 타입들도 불러오고 초기화한다. 여기서는 Init 함수가 호출되어 기본 설정이 초기화된다.
	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>("HitEffect");

	CBullet* PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");

	CCollider* Collider = PlayerBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	CBullet* MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");

	Collider = MonsterBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");


	CPlayer* Player = CreateObject<CPlayer>("Player");
	Player->SetPos(300.0f, 0.0f);
	SetPlayer(Player);

	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CMonster* Monster = CreateObject<CMonster>("Monster",
		Vector2(1000.f, 100.f));

	/*CUIWindow* TestWindow = CreateUIWindow<CUIWindow>("TestWindow");

	CUIImage* Image = TestWindow->CreateWidget<CUIImage>("TestImage");

	Image->SetTexture("Test", TEXT("teemo.bmp"));
	Image->SetPos(100.0f, 100.0f);
	Image->SetZOrder(1);

	CUIImage* Image1 = TestWindow->CreateWidget<CUIImage>("TestImage1");
	
	Image1->SetTexture("Test1", TEXT("Start.bmp"));
	Image1->SetPos(150.0f, 100.0f);*/

	// GetCamera()->SetWorldResolution(3000.0f, 1200.0f);
	// GetCamera()->SetWorldResolution(30000.0f, 12000.0f);

	/*CScrollMap* Map = CreateMap<CScrollMap>("ScrollMap");

	Map->SetSize(1280.0f, 720.0f);
	Map->SetTexture("ScrollBack", TEXT("Sky.bmp"));
	Map->SetLoop(true);
	Map->SetZOrder(0);*/

	// 현재 스크롤 배경의 스크롤 가능한 정도를 구한다.
	// 배경이미지에서 현재 카메라의 해상도를 빼면 구할 수 있음.
	float ScrollWidth = 1500.0f - GetCamera()->GetResolution().x;
	float ScrollHeight = 1200.0f - GetCamera()->GetResolution().y;

	// 타일맵의 전체 스크룰 가능한 정도를 구한다.
	// 현재 타일맵의 크기는 3000, 1200이다
	float TileMapWidth = 3000.0f - GetCamera()->GetResolution().x;
	float TileMapHeight = 1200.0f - GetCamera()->GetResolution().y;

	CScrollMap* Map = CreateMap<CScrollMap>("ScrollMap");

	// 맵의 사이즈를 카메라 해상도에 맞춘다.
	Map->SetSize(1280.0f, 720.0f);
	Map->SetTexture("ScrollBack", TEXT("Sky.bmp"));
	// 맵의 스크롤비율을 비례로 설정한다.
	Map->SetScrollRatio(ScrollWidth / TileMapWidth, ScrollHeight / TileMapHeight);
	Map->SetZOrder(0);

	Map = CreateMap<CScrollMap>("ScrollMap");

	ScrollWidth = 2048.0f - GetCamera()->GetResolution().x;
	ScrollHeight = 2048.0f - GetCamera()->GetResolution().y;

	Map->SetSize(1280.0f, 720.0f);
	Map->SetTexture("Mountain", TEXT("Mountain.bmp"));
	Map->SetScrollRatio(ScrollWidth / TileMapWidth, ScrollHeight / TileMapWidth);
	Map->SetZOrder(1);
	Map->SetTextureColorKey(255, 0, 255);

	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");

	TileMap->LoadFile("MainMap.map");
	TileMap->SetZOrder(2);

	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	return true;
}

bool CMainScene::Render(HDC hDC)
{
	float fps = CGameManager::GetInst()->GetTimer()->GetFPS();
	CScene::Render(hDC);
	std::wstring s = L"FPS : " + std::to_wstring(fps);
	RECT rect;
	rect.left = 1150;
	rect.top = 50;
	SetTextColor(hDC, RGB(0, 255, 0));
	TextOut(hDC, 1000, 100, s.c_str(), s.size());

	return false;
}

void CMainScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("PlayerRightIdle", 
		"PlayerRightIdle", TEXT("Player/Right/astand.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRightIdle", 255, 0, 255);

	for (int i = 0; i < 6; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerRightIdle", i * 82.0f, 0.0f, 82.0f, 73.0f);
	}

	GetSceneResource()->CreateAnimationSequence("PlayerRightWalk", 
		"PlayerRightWalk", TEXT("Player/Right/awalk.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRightWalk", 255, 0, 255);

	for (int i = 0; i < 4; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerRightWalk", i * 85.0f, 0.0f, 85.0f, 75.0f);
	}

	GetSceneResource()->CreateAnimationSequence("PlayerRightAttack", 
		"PlayerRightAttack", TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey("PlayerRightAttack", 255, 0, 255);

	for (int i = 0; i < 3; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerRightAttack", i * 176.0f, 0.0f, 176.0f, 89.0f);
	}

	GetSceneResource()->CreateAnimationSequence("PlayerRightSkill1", 
		"PlayerRightSkill1", TEXT("Player/Right/ashoot1.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerRightSkill1", 255, 0, 255);

	for (int i = 0; i < 3; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerRightSkill1", i * 70.0f, 0.0f, 70.0f, 81.0f);
	}

	GetSceneResource()->CreateAnimationSequence("HitRight", "HitRight", TEXT("Hit2.bmp"));

	GetSceneResource()->SetTextureColorKey("HitRight", 255, 0, 255);

	for (int i = 0; i < 6; i++)
	{
		GetSceneResource()->AddAnimationFrameData("HitRight", i * 178.0f, 0.0f, 178.0f, 164.0f);
	}

	GetSceneResource()->CreateAnimationSequence("Bullet", "Bullet", TEXT("Smoke.bmp"));

	GetSceneResource()->SetTextureColorKey("Bullet", 255, 0, 255);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			GetSceneResource()->AddAnimationFrameData("Bullet", j * 92.0f, i * 92.0f, 92.0f, 92.0f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("PlayerLeftIdle", "PlayerLeftIdle", TEXT("Player/Left/astand_left.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLeftIdle", 255, 0, 255);

	for (int i = 0; i < 6; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerLeftIdle", i * 82.0f, 0.0f, 82.0f, 73.0f);
	}

	GetSceneResource()->CreateAnimationSequence("PlayerLeftWalk", "PlayerLeftWalk", TEXT("Player/Left/awalk_left.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLeftWalk", 255, 0, 255);

	for (int i = 0; i < 4; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerLeftWalk", i * 85.0f, 0.0f, 85.0f, 75.0f);
	}

	GetSceneResource()->CreateAnimationSequence("PlayerLeftAttack", "PlayerLeftAttack", TEXT("Player/Left/aswing_left.bmp"));

	GetSceneResource()->SetTextureColorKey("PlayerLeftAttack", 255, 0, 255);

	for (int i = 0; i < 3; i++)
	{
		GetSceneResource()->AddAnimationFrameData("PlayerLeftAttack", i * 176.0f, 0.0f, 176.0f, 89.0f);
	}
}

void CMainScene::LoadSound()
{
	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");
}
