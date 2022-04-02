#include "CourseSelectUI.h"
#include "../UI/UIImage.h"
#include "../GameManager.h"
#include "UISpriteText.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CCourseSelectUI::CCourseSelectUI() :
	m_BackGround(nullptr),
	m_MushroomCup(nullptr),
	m_FlowerCup(nullptr),
	m_StarCup(nullptr),
	m_SpecialCup(nullptr),
	m_MushroomCupHover(nullptr),
	m_FlowerCupHover(nullptr),
	m_StarCupHover(nullptr),
	m_SpecialCupHover(nullptr),
	m_MushroomIcon(nullptr),
	m_FlowerIcon(nullptr),
	m_StarIcon(nullptr),
	m_CC(nullptr)
{
}

CCourseSelectUI::~CCourseSelectUI()
{
}

bool CCourseSelectUI::Init()
{
	m_Visibility = false;

	Resolution rs = CGameManager::GetInst()->GetResolution();

	m_Pos = Vector2(0.0f, 0.0f);

	// Top = 60, 100 ~ 450, 100
	// Right = 450, 100 ~ 450, 340
	// Bottom = 450, 340 ~ 60, 340
	// Left = 60, 340 ~ 60, 100
	
	// offset 30 pixel

	// 13
	int index = 0;
	for (int i = 0; i < 13; i++)
	{
		Vector2 v = Vector2(60.0f + (i * 30.0f), 100.0f);
		m_IconPosition.push_back(v);
		m_Icons.push_back(std::make_pair(index++, v));
	}

	// 8
	for (int i = 0; i < 8; i++)
	{
		Vector2 v = Vector2(450.0f, 100.0f + (i * 30.0f));
		m_IconPosition.push_back(v);
		m_Icons.push_back(std::make_pair(index++, v));
	}

	// 13
	for (int i = 0; i < 13; i++)
	{
		Vector2 v = Vector2(450.0f - (i * 30.0f), 340.0f);
		m_IconPosition.push_back(v);
		m_Icons.push_back(std::make_pair(index++, v));
	}
	
	// 8
	for (int i = 0; i < 8; i++)
	{
		Vector2 v = Vector2(60.0f, 340.0f - (i * 30.f));
		m_IconPosition.push_back(v);
		m_Icons.push_back(std::make_pair(index++, v));
	}
	
	m_Scene->GetSceneResoruce()->LoadTexture("Mushroom Icon", TEXT("UI/CourseSelect/Mushroom icon.bmp"));
	m_MushroomIcon = m_Scene->GetSceneResoruce()->FindTexture("Mushroom Icon");
	m_MushroomIcon->SetScale(1.5f, 1.5f);

	m_Scene->GetSceneResoruce()->LoadTexture("Flower Icon", TEXT("UI/CourseSelect/Flower icon.bmp"));
	m_FlowerIcon = m_Scene->GetSceneResoruce()->FindTexture("Flower Icon");
	m_FlowerIcon->SetScale(1.5f, 1.5f);

	m_Scene->GetSceneResoruce()->LoadTexture("Star Icon", TEXT("UI/CourseSelect/Star icon.bmp"));
	m_StarIcon = m_Scene->GetSceneResoruce()->FindTexture("Star Icon");
	m_StarIcon->SetScale(1.5f, 1.5f);

	m_BackGround = CreateWidget<CUIImage>("Course select bg");
	m_BackGround->SetZOrder(3);
	m_BackGround->SetTexture("Course select bg", TEXT("UI/CourseSelect/Cup Select bg.bmp"));


	m_MushroomCup = CreateWidget<CUIImage>("Mushroom Cup");
	m_MushroomCup->SetZOrder(1);
	m_MushroomCup->SetTexture("Mushroom Cup", TEXT("UI/CourseSelect/Mushroom.bmp"));
	m_MushroomCup->SetPos(rs.width * 30.0f / 100.0f, rs.height * 30.0f / 100.0f);
	m_MushroomCup->SetScale(1.5f, 1.5f);

	m_MushroomCupHover = CreateWidget<CUIImage>("Mushroom Cup Hover");
	m_MushroomCupHover->SetZOrder(1);
	m_MushroomCupHover->SetTexture("Mushroom Cup Hover", TEXT("UI/CourseSelect/Mushroom hover.bmp"));
	m_MushroomCupHover->SetPos(rs.width * 30.0f / 100.0f, rs.height * 30.0f / 100.0f);
	m_MushroomCupHover->SetScale(1.5f, 1.5f);
	m_MushroomCupHover->SetVisibility(false);

	m_FlowerCup = CreateWidget<CUIImage>("Flower Cup");
	m_FlowerCup->SetZOrder(1);
	m_FlowerCup->SetTexture("Flower Cup", TEXT("UI/CourseSelect/Flower.bmp"));
	m_FlowerCup->SetPos(rs.width * 30.0f / 100.0f, rs.height * 40.0f / 100.0f);
	m_FlowerCup->SetScale(1.5f, 1.5f);

	m_FlowerCupHover = CreateWidget<CUIImage>("Flower Cup Hover");
	m_FlowerCupHover->SetZOrder(1);
	m_FlowerCupHover->SetTexture("Flower Cup Hover", TEXT("UI/CourseSelect/Flower hover.bmp"));
	m_FlowerCupHover->SetPos(rs.width * 30.0f / 100.0f, rs.height * 40.0f / 100.0f);
	m_FlowerCupHover->SetScale(1.5f, 1.5f);
	m_FlowerCupHover->SetVisibility(false);

	m_StarCup = CreateWidget<CUIImage>("Star Cup");
	m_StarCup->SetZOrder(1);
	m_StarCup->SetTexture("Star Cup", TEXT("UI/CourseSelect/Star.bmp"));
	m_StarCup->SetPos(rs.width * 30.0f / 100.0f, rs.height * 50.0f / 100.0f);
	m_StarCup->SetScale(1.5f, 1.5f);

	m_StarCupHover = CreateWidget<CUIImage>("Star Cup Hover");
	m_StarCupHover->SetZOrder(1);
	m_StarCupHover->SetTexture("Star Cup Hover", TEXT("UI/CourseSelect/Star hover.bmp"));
	m_StarCupHover->SetPos(rs.width * 30.0f / 100.0f, rs.height * 50.0f / 100.0f);
	m_StarCupHover->SetScale(1.5f, 1.5f);
	m_StarCupHover->SetVisibility(false);

	m_SpecialCup = CreateWidget<CUIImage>("Special Cup");
	m_SpecialCup->SetZOrder(1);
	m_SpecialCup->SetTexture("Special Cup", TEXT("UI/CourseSelect/Special.bmp"));
	m_SpecialCup->SetPos(rs.width * 30.0f / 100.0f, rs.height * 60.0f / 100.0f);
	m_SpecialCup->SetScale(1.5f, 1.5f);

	m_SpecialCupHover = CreateWidget<CUIImage>("Special Cup Hover");
	m_SpecialCupHover->SetZOrder(1);
	m_SpecialCupHover->SetTexture("Special Cup Hover", TEXT("UI/CourseSelect/Special hover.bmp"));
	m_SpecialCupHover->SetPos(rs.width * 30.0f / 100.0f, rs.height * 60.0f / 100.0f);
	m_SpecialCupHover->SetScale(1.5f, 1.5f);
	m_SpecialCupHover->SetVisibility(false);


	SortWidget();

	return true;
}

void CCourseSelectUI::Render(HDC hDC)
{
	CUIWindow::Render(hDC);

	for (size_t i = 0; i < m_Icons.size(); i++)
	{
		switch (m_CurrentIcon)
		{
		case CCourseSelectUI::EIcon::Mushroom:
			m_MushroomIcon->Render(hDC, m_Icons[i].second, Vector2(0.0f, 0.0f), Vector2(m_MushroomIcon->GetWidth(), m_MushroomIcon->GetHeight()));
			break;
		case CCourseSelectUI::EIcon::Flower:
			m_FlowerIcon->Render(hDC, m_Icons[i].second, Vector2(0.0f, 0.0f), Vector2(m_FlowerIcon->GetWidth(), m_FlowerIcon->GetHeight()));
			break;
		case CCourseSelectUI::EIcon::Star:
			m_StarIcon->Render(hDC, m_Icons[i].second, Vector2(0.0f, 0.0f), Vector2(m_StarIcon->GetWidth(), m_StarIcon->GetHeight()));
			break;
		case CCourseSelectUI::EIcon::Special:
		{
			switch (i % 3)
			{
			case 0:
				m_MushroomIcon->Render(hDC, m_Icons[i].second, Vector2(0.0f, 0.0f), Vector2(m_MushroomIcon->GetWidth(), m_MushroomIcon->GetHeight()));
				break;
			case 1:
				m_FlowerIcon->Render(hDC, m_Icons[i].second, Vector2(0.0f, 0.0f), Vector2(m_FlowerIcon->GetWidth(), m_FlowerIcon->GetHeight()));
				break;
			case 2:
				m_StarIcon->Render(hDC, m_Icons[i].second, Vector2(0.0f, 0.0f), Vector2(m_StarIcon->GetWidth(), m_StarIcon->GetHeight()));
				break;
			}
		}
		}
	}
}

void CCourseSelectUI::Update(float deltaTime)
{

	CUIWindow::Update(deltaTime);

	auto GetSquaredDist = [](const Vector2& from, const Vector2& to)
	{
		float x = from.x - to.x;
		float y = from.y - to.y;

		return x * x + y * y;
	};

	for (size_t i = 0; i < m_Icons.size(); i++)
	{
		if (GetSquaredDist(m_Icons[i].second, m_IconPosition[m_Icons[i].first]) < 4.0f)
		{
			m_Icons[i].first = (m_Icons[i].first + 1) % m_Icons.size();
		}

		Vector2 dir = m_IconPosition[m_Icons[i].first] - m_Icons[i].second;

		dir.Normalize();

		m_Icons[i].second += dir * (deltaTime * 10.0f);
	}
}

void CCourseSelectUI::SetCCText(ECCOption option)
{
	switch (option)
	{
	case ECCOption::CC50:
		m_CC = CreateWidget<CUISpriteText>("CC Text");
		m_CC->SetText("50 CC");
		m_CC->SetPos(219.0f, 45.0f);
		m_CC->SetZOrder(1);
		break;
	case ECCOption::CC100:
		m_CC = CreateWidget<CUISpriteText>("CC Text");
		m_CC->SetText("100 CC");
		m_CC->SetPos(219.0f, 45.0f);
		m_CC->SetZOrder(1);
		break;
	case ECCOption::CC150:
		m_CC = CreateWidget<CUISpriteText>("CC Text");
		m_CC->SetText("150 CC");
		m_CC->SetPos(219.0f, 45.0f);
		m_CC->SetZOrder(1);
		break;
	case ECCOption::CC200:
		m_CC = CreateWidget<CUISpriteText>("CC Text");
		m_CC->SetText("200 CC");
		m_CC->SetPos(219.0f, 45.0f);
		m_CC->SetZOrder(1);
		break;
	}
}

void CCourseSelectUI::SetMushroomCup()
{
	m_MushroomCupHover->SetVisibility(true);
	m_MushroomCup->SetVisibility(false);

	m_FlowerCupHover->SetVisibility(false);
	m_FlowerCup->SetVisibility(true);
	
	m_StarCupHover->SetVisibility(false);
	m_StarCup->SetVisibility(true);

	m_SpecialCupHover->SetVisibility(false);
	m_SpecialCup->SetVisibility(true);

	m_CurrentIcon = EIcon::Mushroom;
}

void CCourseSelectUI::SetFlowerCup()
{
	m_MushroomCupHover->SetVisibility(false);
	m_MushroomCup->SetVisibility(true);

	m_FlowerCupHover->SetVisibility(true);
	m_FlowerCup->SetVisibility(false);

	m_StarCupHover->SetVisibility(false);
	m_StarCup->SetVisibility(true);

	m_SpecialCupHover->SetVisibility(false);
	m_SpecialCup->SetVisibility(true);

	m_CurrentIcon = EIcon::Flower;
}

void CCourseSelectUI::SetStarCup()
{
	m_MushroomCupHover->SetVisibility(false);
	m_MushroomCup->SetVisibility(true);

	m_FlowerCupHover->SetVisibility(false);
	m_FlowerCup->SetVisibility(true);

	m_StarCupHover->SetVisibility(true);
	m_StarCup->SetVisibility(false);

	m_SpecialCupHover->SetVisibility(false);
	m_SpecialCup->SetVisibility(true);

	m_CurrentIcon = EIcon::Star;
}

void CCourseSelectUI::SetSpecialCup()
{
	m_MushroomCupHover->SetVisibility(false);
	m_MushroomCup->SetVisibility(true);

	m_FlowerCupHover->SetVisibility(false);
	m_FlowerCup->SetVisibility(true);

	m_StarCupHover->SetVisibility(false);
	m_StarCup->SetVisibility(true);

	m_SpecialCupHover->SetVisibility(true);
	m_SpecialCup->SetVisibility(false);

	m_CurrentIcon = EIcon::Special;
}
