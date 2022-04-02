#include "TitleScene.h"
#include "../Map/CScrollMap.h"
#include "../Scene/Camera.h"
#include "../UI/TitleUI.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "EditorScene.h"
#include "Mode7Demo.h"
#include "SelectScene.h"
#include "../UI/RaceMenuUI.h"
#include "../Input.h"
#include "../GameManager.h"

CTitleScene::CTitleScene() :
	m_Start(false),
	m_TitleUI(nullptr),
	m_ShowMenu(false),
	m_ShowRaceMenu(false),
	m_CurrentMenu(EMenuState::RACE),
	m_CCSelect(ECCOption::CC50),
	m_CurrentTime(0.0f),
	m_RaceMenuUI(nullptr)
{
}

CTitleScene::~CTitleScene()
{
}

bool CTitleScene::Init()
{
	LoadSound();

	// 스크롤 배경을 만들어낸다.
	m_ScrollMap = CreateMap<CScrollMap>("ScrollBG");

	m_ScrollMap->SetTexture("BG", TEXT("Menu/BG.bmp"));

	// 크기를 설정한다.
	m_ScrollMap->SetSize(512.0f, 448.0f);

	// 루프로 설정
	m_ScrollMap->SetLoop(true);

	// 타이틀 UI를 만들어낸다.
	m_TitleUI = CreateUIWindow<CTitleUI>("Title UI");

	m_TitleUI->SetZOrder(2);

	m_RaceMenuUI = CreateUIWindow<CRaceMenuUI>("Race menu UI");

	m_RaceMenuUI->SetZOrder(1);

	// 카메라의 경우 경계선을 넘어도 다시 0, 0으로 돌아오도록 나머지 연산을 통해 구현한다.
	GetCamera()->SetInfinite(true);

	m_CursorPos = m_TitleUI->GetRaceTextPos();

	m_CursorPos.x -= 10.0f;

	CGameObject* driver;
	driver = CreateObject<CGameObject>("Mario");

	driver->SetTexture("Mario", TEXT("Character/Mario/RMario8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);


	driver = CreateObject<CGameObject>("Bowser");

	driver->SetTexture("Bowser", TEXT("Character/Bowser/RBowser8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);

	driver = CreateObject<CGameObject>("DK");

	driver->SetTexture("DK", TEXT("Character/DK/RDK8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);

	driver = CreateObject<CGameObject>("Koopa");

	driver->SetTexture("Koopa", TEXT("Character/Koopa/RKoopa8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);

	driver = CreateObject<CGameObject>("Luigi");

	driver->SetTexture("Luigi", TEXT("Character/Luigi/RLuigi8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);

	driver = CreateObject<CGameObject>("Peach");

	driver->SetTexture("Peach", TEXT("Character/Peach/RPeach8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);

	driver = CreateObject<CGameObject>("Toad");

	driver->SetTexture("Toad", TEXT("Character/Toad/RToad8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);

	driver = CreateObject<CGameObject>("Yoshi");

	driver->SetTexture("Yoshi", TEXT("Character/Yoshi/RYoshi8.bmp"));

	driver->SetTextureColorKey(255, 0, 255);

	driver->GetTexture()->SetScale(2.0f, 2.0f);

	driver->SetPivot(0.5f, 1.0f);


	return true;
}

bool CTitleScene::Update(float deltaTime)
{
	m_CurrentTime += deltaTime;

	if (!m_Start)
	{
		// 초기 키 콜백함수와 기타 설정등을 초기화한다.
		m_Start = true;
		CInput::GetInst()->SetMenuModeKeySet();

		CInput::GetInst()->SetCallback<CTitleScene>("Up", KeyState_Down, this, &CTitleScene::MoveCursorUp);
		CInput::GetInst()->SetCallback<CTitleScene>("Down", KeyState_Down, this, &CTitleScene::MoveCursorDown);
		CInput::GetInst()->SetCallback<CTitleScene>("Next", KeyState_Down, this, &CTitleScene::Next);
		CInput::GetInst()->SetCallback<CTitleScene>("Back", KeyState_Down, this, &CTitleScene::Back);

		Resolution rs = CGameManager::GetInst()->GetResolution();

		int index = 0;
		for (auto& obj : m_ObjList)
		{
			obj->SetPos(Vector2(-rand() % 200 - 50, rand() % 224 + 240));
			m_DriverFrequency[index++] = ((rand() % 51 + 50) / (float)100) * 20.0f;
			obj->SetMoveSpeed(rand() % 50 + 10);
		}

		GetSceneResoruce()->SoundPlay("Title BGM");
		GetSceneResoruce()->SoundStop("player_circuit");
	}

	CScene::Update(deltaTime);

	// 메뉴가 보이지 않을시에는 드라이버들을 움직이게한다.
	if (!m_ShowMenu)
	{
		GetCamera()->Move(Vector2(50.0f * deltaTime, 0.0f));

		int index = 0;
		for (auto& obj : m_ObjList)
		{
			if (obj->GetPos().x > (550.0f))
			{
				obj->SetPos(Vector2(-rand() % 300 - 150, rand() % 224 + 240));
				m_DriverFrequency[index++] = ((rand() % 51 + 50) / (float)100) * 20.0f;
				obj->SetMoveSpeed(rand() % 50 + 10);
			}
			else
			{
				obj->Move(Vector2(1.0, 0.0f));
				obj->SetPos(obj->GetPos().x, obj->GetPivotY() + cosf(m_DriverFrequency[index++] * m_CurrentTime));
			}
		}
	}

	return false;
}

bool CTitleScene::Render(HDC hDC)
{
	CScene::Render(hDC);

	// 커서를 그려낸다.
	if (m_ShowMenu)
	{
		CTexture* cursor = GetSceneResoruce()->FindSpriteFont("Right");
		cursor->Render(hDC, m_CursorPos, Vector2(0.0f, 0.0f), Vector2(9.0f, 9.0f));
	}

	return false;
}

void CTitleScene::LoadSound()
{
	GetSceneResoruce()->LoadSound("BGM", true, "Title BGM", "menu/menu_title_screen.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Menu Select", "menu/menuselect.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Menu Move", "menu/menumove.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "Menu Back", "menu/menuback.ogg");
}

void CTitleScene::MoveCursorUp(float deltaTime)
{
	if (!m_ShowMenu)
		return;

	GetSceneResoruce()->SoundPlay("Menu Move");

	if (!m_ShowRaceMenu)
	{
		if (m_CurrentMenu == EMenuState::RACE)
			return;

		m_CurrentMenu = EMenuState((int)m_CurrentMenu - 1);

		switch (m_CurrentMenu)
		{
		case CTitleScene::EMenuState::RACE:
		{
			m_CursorPos = m_TitleUI->GetRaceTextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case CTitleScene::EMenuState::TOOL:
		{
			m_CursorPos = m_TitleUI->GetToolTextPos();
			m_CursorPos.x -= 10.f;
		}
		break;
		case CTitleScene::EMenuState::MODE7:
		{
			m_CursorPos = m_TitleUI->GetMode7TextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case CTitleScene::EMenuState::EXIT:
		{
			m_CursorPos = m_TitleUI->GetExitTextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		}
	}
	else
	{
		if (m_CCSelect == ECCOption::CC50)
			return;

		m_CCSelect = ECCOption((int)m_CCSelect - 1);

		switch (m_CCSelect)
		{
		case ECCOption::CC50:
		{
			m_CursorPos = m_RaceMenuUI->GetCC50Pos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case ECCOption::CC100:
		{
			m_CursorPos = m_RaceMenuUI->GetCC100Pos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case ECCOption::CC150:
		{
			m_CursorPos = m_RaceMenuUI->GetCC150Pos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case ECCOption::CC200:
		{
			m_CursorPos = m_RaceMenuUI->GetCC200Pos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		}
	}
}

void CTitleScene::MoveCursorDown(float deltaTime)
{
	if (!m_ShowMenu)
		return;

	GetSceneResoruce()->SoundPlay("Menu Move");

	if (!m_ShowRaceMenu)
	{
		if (m_CurrentMenu == EMenuState::EXIT)
			return;

		m_CurrentMenu = EMenuState((int)m_CurrentMenu + 1);

		switch (m_CurrentMenu)
		{
		case CTitleScene::EMenuState::RACE:
		{
			m_CursorPos = m_TitleUI->GetRaceTextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case CTitleScene::EMenuState::TOOL:
		{
			m_CursorPos = m_TitleUI->GetToolTextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		case CTitleScene::EMenuState::MODE7:
		{
			m_CursorPos = m_TitleUI->GetMode7TextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;

		case CTitleScene::EMenuState::EXIT:
		{
			m_CursorPos = m_TitleUI->GetExitTextPos();
			m_CursorPos.x -= 10.0f;
		}
		break;
		}
	}
	else
	{
		if (m_CCSelect == ECCOption::CC200)
			return;

		m_CCSelect = ECCOption((int)m_CCSelect + 1);

		switch (m_CCSelect)
		{
		case ECCOption::CC50:
		{
			m_CursorPos = m_RaceMenuUI->GetCC50Pos();
			m_CursorPos.x -= 10.0f;
		}
			break;
		case ECCOption::CC100:
		{
			m_CursorPos = m_RaceMenuUI->GetCC100Pos();
			m_CursorPos.x -= 10.0f;
		}
			break;
		case ECCOption::CC150:
		{
			m_CursorPos = m_RaceMenuUI->GetCC150Pos();
			m_CursorPos.x -= 10.0f;
		}
			break;
		case ECCOption::CC200:
		{
			m_CursorPos = m_RaceMenuUI->GetCC200Pos();
			m_CursorPos.x -= 10.0f;
		}
			break;
		}
	}
}

void CTitleScene::Next(float deltaTime)
{
	if (!m_ShowMenu)
	{
		m_ShowMenu = true;
		GetSceneResoruce()->SoundPlay("Menu Select");
		m_TitleUI->ToggleShow();
	}
	else
	{
		if (!m_ShowRaceMenu)
		{
			switch (m_CurrentMenu)
			{
			case EMenuState::RACE:
				GetSceneResoruce()->SoundPlay("Menu Select");
				m_RaceMenuUI->ToggleVisibility();
				m_CursorPos = m_RaceMenuUI->GetCC50Pos();
				m_CursorPos.x -= 10.0f;
				m_ShowRaceMenu = true;
				break;
			case EMenuState::TOOL:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CSceneManager::GetInst()->CreateScene<CEditorScene>();
				break;
			case EMenuState::MODE7:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CSceneManager::GetInst()->CreateScene<CMode7Demo>();
				break;
			case EMenuState::EXIT:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CGameManager::GetInst()->Exit();
				break;
			}
		}
		else
		{
			switch (m_CCSelect)
			{
			case ECCOption::CC50:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CSceneManager::GetInst()->CreateSelectScene<CSelectScene>(ECCOption::CC50);
				break;
			case ECCOption::CC100:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CSceneManager::GetInst()->CreateSelectScene<CSelectScene>(ECCOption::CC100);
				break;
			case ECCOption::CC150:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CSceneManager::GetInst()->CreateSelectScene<CSelectScene>(ECCOption::CC150);
				break;
			case ECCOption::CC200:
				GetSceneResoruce()->SoundPlay("Menu Select");
				CSceneManager::GetInst()->CreateSelectScene<CSelectScene>(ECCOption::CC200);
				break;
			}
		}
	}
}

void CTitleScene::Back(float deltaTime)
{
	if (m_ShowRaceMenu)
	{
		m_RaceMenuUI->ToggleVisibility();
		GetSceneResoruce()->SoundPlay("Menu Back");
		m_CursorPos = m_TitleUI->GetRaceTextPos();
		m_CursorPos.x -= 10.0f;
		m_ShowRaceMenu = false;
		m_CCSelect = ECCOption::CC50;
		return;
	}


	if (m_ShowMenu)
	{
		m_ShowMenu = false;
		GetSceneResoruce()->SoundPlay("Menu Back");
		m_CCSelect = ECCOption::CC50;
		m_TitleUI->ToggleShow();
	}
}
