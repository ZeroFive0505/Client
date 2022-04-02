#include "SelectScene.h"
#include "../UI/UIImage.h"
#include "../UI/DriverSelectUI.h"
#include "../UI/CourseSelectUI.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "RaceScene.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../UI/CourseWindow.h"


CSelectScene::CSelectScene() :
	m_DriverSelectUI(nullptr),
	m_CourseSelectUI(nullptr),
	m_Start(false),
	m_CurrentDriver(EMenuPlayer::MARIO),
	m_DriverSelected(false),
	m_fElapsedTime(0.0f),
	m_FadeOut(false),
	m_FadeIn(false),
	m_UIState(EUIState::DRIVER),
	m_CurrentCup(ECupSelect::MUSHROOM),
	m_CupSelected(false),
	m_MushroomWindow(nullptr),
	m_FlowerWindow(nullptr),
	m_StarWindow(nullptr),
	m_SpecialWindow(nullptr),
	m_CourseIndex(0)
{
}

CSelectScene::~CSelectScene()
{
}

bool CSelectScene::Init()
{
	LoadSound();

	Resolution rs = CGameManager::GetInst()->GetResolution();

	m_DriverSelectUI = CreateUIWindow<CDriverSelectUI>("Driver Select UI");

	m_CourseSelectUI = CreateUIWindow<CCourseSelectUI>("Course Select UI");

	m_MushroomWindow = CreateUIWindow<CCourseWindow>("Mushroom Course Window");

	m_MushroomWindow->AddCircuit("Ghost valley1", "Ghost valley1", Vector2(rs.width * 40.0f / 100.0f, rs.height * 40.0f / 100.0f));
	m_MushroomWindow->AddCircuit("Mario circuit2", "Mario circuit2", Vector2(rs.width * 40.0f / 100.0f, rs.height * 45.0f / 100.0f));

	m_FlowerWindow = CreateUIWindow<CCourseWindow>("Flower Course Window");

	m_FlowerWindow->AddCircuit("Choco island1", "Choco island1", Vector2(rs.width * 40.0f / 100.0f, rs.height * 40.0f / 100.0f));

	m_StarWindow = CreateUIWindow<CCourseWindow>("Star Course Window");

	m_StarWindow->AddCircuit("Bowser castle3", "Bowser castle3", Vector2(rs.width * 40.0f / 100.0f, rs.height * 40.0f / 100.0f));

	m_SpecialWindow = CreateUIWindow<CCourseWindow>("Special Course Window");

	m_SpecialWindow->AddCircuit("Rainbowroad", "Rainbowroad", Vector2(rs.width * 40.0f / 100.0f, rs.height * 40.0f / 100.0f));

	GetSceneResoruce()->LoadTexture("1P", TEXT("UI/DriverSelect/1P.bmp"));

	m_1P = GetSceneResoruce()->FindTexture("1P");

	m_1P->SetPos(m_PlayerOnePos[0]);

	m_1P->SetScale(2.0f, 2.0f);

	return true;
}

bool CSelectScene::Update(float deltaTime)
{
	if (!m_Start)
	{
		m_Start = true;

		GetSceneResoruce()->SoundPlay("player_circuit");

		CInput::GetInst()->SetMenuModeKeySet();

		GetSceneResoruce()->SoundStop("Title BGM");

		CInput::GetInst()->SetCallback<CSelectScene>("Up", KeyState_Down, this, &CSelectScene::Up);
		CInput::GetInst()->SetCallback<CSelectScene>("Down", KeyState_Down, this, &CSelectScene::Down);
		CInput::GetInst()->SetCallback<CSelectScene>("Left", KeyState_Down, this, &CSelectScene::Left);
		CInput::GetInst()->SetCallback<CSelectScene>("Right", KeyState_Down, this, &CSelectScene::Right);

		CInput::GetInst()->SetCallback<CSelectScene>("Next", KeyState_Down, this, &CSelectScene::Next);
		CInput::GetInst()->SetCallback<CSelectScene>("Back", KeyState_Down, this, &CSelectScene::Back);
	}

	if (m_FadeOut || m_FadeIn)
	{
		m_fElapsedTime += deltaTime;
	}

	CScene::Update(deltaTime);

	return false;
}

bool CSelectScene::Render(HDC hDC)
{
	CScene::Render(hDC);

	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if (!m_FadeIn && !m_FadeOut)
			m_1P->Render(hDC, Vector2(0.0f, 0.0f), Vector2(m_1P->GetWidth(), m_1P->GetHeight()));
	}
	break;
	case CSelectScene::EUIState::COURSE:
	{
		if (m_CupSelected)
		{
			CTexture* cursor = GetSceneResoruce()->FindSpriteFont("Right");
			cursor->Render(hDC, m_CursorPos, Vector2(0.0f, 0.0f), Vector2(cursor->GetWidth(), cursor->GetHeight()));
		}
	}
	break;
	default:
		break;
	}


	if (m_FadeOut)
	{
		float pct = fminf(m_fElapsedTime / UI_FADEOUT_TIME, 1.0f);

		m_FadeLT.x = 256.0f - (255.0f * pct);
		m_FadeLT.y = 224.0f - (223.0f * pct);
		m_FadeRB.x = 256.0f + (255.0f * pct);
		m_FadeRB.y = 224.0f + (223.0f * pct);
		RECT rc;
		rc.left = (LONG)m_FadeLT.x;
		rc.top = (LONG)m_FadeLT.y;
		rc.right = (LONG)m_FadeRB.x;
		rc.bottom = (LONG)m_FadeRB.y;
		FillRect(hDC, &rc, CGameManager::GetInst()->GetBlackBrush());

		if (pct >= 1.0f)
		{
			m_FadeOut = false;
			m_FadeIn = true;
			m_fElapsedTime = 0.0f;
		}
	}
	else if (m_FadeIn)
	{
		float pct = fminf(m_fElapsedTime / UI_FADEIN_TIME, 1.0f);

		m_FadeLT.x = 0.0f + (255.0f * pct);
		m_FadeLT.y = 0.0f + (223.0f * pct);
		m_FadeRB.x = 512.0f - (255.0f * pct);
		m_FadeRB.y = 448.0f - (223.0f * pct);

		RECT rc;
		rc.left = (LONG)m_FadeLT.x;
		rc.top = (LONG)m_FadeLT.y;
		rc.right = (LONG)m_FadeRB.x;
		rc.bottom = (LONG)m_FadeRB.y;
		FillRect(hDC, &rc, CGameManager::GetInst()->GetBlackBrush());

		switch (m_UIState)
		{
		case CSelectScene::EUIState::DRIVER:
		{
			m_DriverSelectUI->SetVisibility(true);
			m_CourseSelectUI->SetVisibility(false);
		}
		break;
		case CSelectScene::EUIState::COURSE:
		{
			m_DriverSelectUI->SetVisibility(false);
			m_CurrentCup = ECupSelect::MUSHROOM;
			m_CourseSelectUI->SetMushroomCup();
			m_CourseSelectUI->SetVisibility(true);
		}
		break;
		}

		if (pct >= 1.0f)
		{
			m_FadeIn = false;
			m_fElapsedTime = 0.0f;
		}
	}

	return false;
}

void CSelectScene::SetCCText(ECCOption option)
{
	m_CourseSelectUI->SetCCText(option);
}

void CSelectScene::LoadSound()
{
	GetSceneResoruce()->LoadSound("BGM", true, "player_circuit", "menu/menu_player_circuit.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "select", "menu/menuselect.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "cancel", "menu/menuback.ogg");
	GetSceneResoruce()->LoadSound("Effect", false, "move", "menu/menumove.ogg");
}

void CSelectScene::Up(float deltaTime)
{
	if (m_FadeOut || m_FadeIn)
		return;


	GetSceneResoruce()->SoundPlay("move");

	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if ((int)m_CurrentDriver - 4 < 0)
			return;

		if (m_DriverSelected)
			return;

		m_CurrentDriver = EMenuPlayer((int)m_CurrentDriver - 4);
		m_1P->SetPos(m_PlayerOnePos[(int)m_CurrentDriver]);
	}
	break;
	case CSelectScene::EUIState::COURSE:
	{
		switch (m_CurrentCup)
		{
		case ECupSelect::MUSHROOM:
		{
			if (m_CupSelected)
			{
				if (m_CourseIndex != 0)
				{
					m_CourseIndex--;
					m_CursorPos = m_MushroomWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
			break;
		case ECupSelect::FLOWER:
		{
			if (!m_CupSelected)
			{
				m_CurrentCup = ECupSelect::MUSHROOM;
				m_CourseSelectUI->SetMushroomCup();
			}
			else
			{
				if (m_CourseIndex != 0)
				{
					m_CourseIndex--;
					m_CursorPos = m_FlowerWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
		break;
		case ECupSelect::STAR:
		{
			if (!m_CupSelected)
			{
				m_CurrentCup = ECupSelect::FLOWER;
				m_CourseSelectUI->SetFlowerCup();
			}
			else
			{
				if (m_CourseIndex != 0)
				{
					m_CourseIndex--;
					m_CursorPos = m_StarWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
		break;
		case ECupSelect::SEPCIAL:
		{
			if (!m_CupSelected)
			{
				m_CurrentCup = ECupSelect::STAR;
				m_CourseSelectUI->SetStarCup();
			}
			else
			{
				if (m_CourseIndex != 0)
				{
					m_CourseIndex--;
					m_CursorPos = m_SpecialWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
		break;
		}
	}
	break;
	}

}

void CSelectScene::Down(float deltaTime)
{
	if (m_FadeOut || m_FadeIn)
		return;

	GetSceneResoruce()->SoundPlay("move");

	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if ((int)m_CurrentDriver + 4 > 7)
			return;

		if (m_DriverSelected)
			return;

		m_CurrentDriver = EMenuPlayer((int)m_CurrentDriver + 4);
		m_1P->SetPos(m_PlayerOnePos[(int)m_CurrentDriver]);
	}
	break;
	case CSelectScene::EUIState::COURSE:
	{

		switch (m_CurrentCup)
		{
		case ECupSelect::MUSHROOM:
		{
			if (!m_CupSelected)
			{
				m_CurrentCup = ECupSelect::FLOWER;
				m_CourseSelectUI->SetFlowerCup();
			}
			else
			{
				if ((m_CourseIndex + 1) != m_MushroomWindow->GetCourseSize())
				{
					m_CourseIndex++;
					m_CursorPos = m_MushroomWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
		break;
		case ECupSelect::FLOWER:
		{
			if (!m_CupSelected)
			{
				m_CurrentCup = ECupSelect::STAR;
				m_CourseSelectUI->SetStarCup();
			}
			else
			{
				if ((m_CourseIndex + 1) != m_FlowerWindow->GetCourseSize())
				{
					m_CourseIndex++;
					m_CursorPos = m_FlowerWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
		break;
		case ECupSelect::STAR:
		{
			if (!m_CupSelected)
			{
				m_CurrentCup = ECupSelect::SEPCIAL;
				m_CourseSelectUI->SetSpecialCup();
			}
			else
			{
				if ((m_CourseIndex + 1) != m_StarWindow->GetCourseSize())
				{
					m_CourseIndex++;
					m_CursorPos = m_StarWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
		break;
		case ECupSelect::SEPCIAL:
		{
			if (m_CupSelected)
			{
				if ((m_CourseIndex + 1) != m_SpecialWindow->GetCourseSize())
				{
					m_CourseIndex++;
					m_CursorPos = m_SpecialWindow->GetCursorPos(m_CourseIndex);
					m_CursorPos.x -= 10.0f;
				}
			}
		}
			break;
		}

	}
	break;
	}
}

void CSelectScene::Left(float deltaTime)
{
	if (m_FadeOut || m_FadeIn)
		return;



	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if ((int)m_CurrentDriver == 0 || (int)m_CurrentDriver == 4)
			return;
		if (m_DriverSelected)
			return;
		GetSceneResoruce()->SoundPlay("move");
		m_CurrentDriver = EMenuPlayer((int)m_CurrentDriver - 1);
		m_1P->SetPos(m_PlayerOnePos[(int)m_CurrentDriver]);
	}
	break;
	case CSelectScene::EUIState::COURSE:
		break;
	}
}

void CSelectScene::Right(float deltaTime)
{
	if (m_FadeOut || m_FadeIn)
		return;


	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if ((int)m_CurrentDriver == 3 || (int)m_CurrentDriver == 7)
			return;
		if (m_DriverSelected)
			return;
		GetSceneResoruce()->SoundPlay("move");
		m_CurrentDriver = EMenuPlayer((int)m_CurrentDriver + 1);
		m_1P->SetPos(m_PlayerOnePos[(int)m_CurrentDriver]);
	}
	break;
	case CSelectScene::EUIState::COURSE:
		break;
	}
}

void CSelectScene::Next(float deltaTime)
{
	if (m_FadeOut || m_FadeIn)
		return;

	GetSceneResoruce()->SoundPlay("select");

	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if (!m_DriverSelectUI->IsSelected((int)m_CurrentDriver))
		{
			m_DriverSelectUI->Select((int)m_CurrentDriver);
			m_DriverSelected = true;
		}
		else
		{
			m_DriverSelectUI->Confirm((int)m_CurrentDriver);
			m_UIState = EUIState::COURSE;
			m_fElapsedTime = 0.0f;
			m_FadeOut = true;
		}
	}
	break;
	case CSelectScene::EUIState::COURSE:
	{

		switch (m_CurrentCup)
		{
		case ECupSelect::MUSHROOM:
		{
			if (!m_CupSelected)
			{
				m_CupSelected = true;
				m_CourseIndex = 0;
				m_CursorPos = m_MushroomWindow->GetCursorPos(m_CourseIndex);
				m_CursorPos.x -= 10.0f;
				m_MushroomWindow->SetVisibility(true);
			}
			else
			{
				CSceneManager::GetInst()->CreateRaceScene<CRaceScene>(m_CurrentCCOption, m_CurrentCup, m_MushroomWindow->GetCurrentCourse(m_CourseIndex),
					m_CurrentDriver);
			}
		}
		break;
		case ECupSelect::FLOWER:
		{
			if (!m_CupSelected)
			{
				m_CupSelected = true;
				m_CourseIndex = 0;
				m_CursorPos = m_MushroomWindow->GetCursorPos(m_CourseIndex);
				m_CursorPos.x -= 10.0f;
				m_FlowerWindow->SetVisibility(true);
			}
			else
			{
				CSceneManager::GetInst()->CreateRaceScene<CRaceScene>(m_CurrentCCOption, m_CurrentCup, m_FlowerWindow->GetCurrentCourse(m_CourseIndex),
					m_CurrentDriver);
			}
		}
		break;
		case ECupSelect::STAR:
		{
			if (!m_CupSelected)
			{
				m_CupSelected = true;
				m_CourseIndex = 0;
				m_CursorPos = m_MushroomWindow->GetCursorPos(m_CourseIndex);
				m_CursorPos.x -= 10.0f;
				m_StarWindow->SetVisibility(true);
			}
			else
			{
				CSceneManager::GetInst()->CreateRaceScene<CRaceScene>(m_CurrentCCOption, m_CurrentCup, m_StarWindow->GetCurrentCourse(m_CourseIndex),
					m_CurrentDriver);
			}
		}
		break;
		case ECupSelect::SEPCIAL:
		{
			if (!m_CupSelected)
			{
				m_CupSelected = true;
				m_CourseIndex = 0;
				m_CursorPos = m_MushroomWindow->GetCursorPos(m_CourseIndex);
				m_CursorPos.x -= 10.0f;
				m_SpecialWindow->SetVisibility(true);
			}
			else
			{
				CSceneManager::GetInst()->CreateRaceScene<CRaceScene>(m_CurrentCCOption, m_CurrentCup, m_SpecialWindow->GetCurrentCourse(m_CourseIndex),
					m_CurrentDriver);
			}
		}
		break;
		}

	}
	break;
	}
}

void CSelectScene::Back(float deltaTime)
{
	if (m_FadeOut || m_FadeIn)
		return;

	GetSceneResoruce()->SoundPlay("cancel");

	switch (m_UIState)
	{
	case CSelectScene::EUIState::DRIVER:
	{
		if (m_DriverSelected)
		{
			m_DriverSelected = false;
			m_DriverSelectUI->Cancel();
		}
		else
		{
			CSceneManager::GetInst()->CreateScene<CTitleScene>();
		}
	}
	break;
	case CSelectScene::EUIState::COURSE:
	{
		if (!m_CupSelected)
		{
			m_UIState = EUIState::DRIVER;
			m_DriverSelectUI->ResetConfirm((int)m_CurrentDriver);
			m_FadeOut = true;
			m_fElapsedTime = 0.0f;
		}
		else
		{
			m_CupSelected = false;

			switch (m_CurrentCup)
			{
			case ECupSelect::MUSHROOM:
				m_MushroomWindow->SetVisibility(false);
				break;
			case ECupSelect::FLOWER:
				m_FlowerWindow->SetVisibility(false);
				break;
			case ECupSelect::STAR:
				m_StarWindow->SetVisibility(false);
				break;
			case ECupSelect::SEPCIAL:
				m_SpecialWindow->SetVisibility(false);
				break;
			}
		}
	}
	break;
	}
}
