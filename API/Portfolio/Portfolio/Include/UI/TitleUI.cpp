#include "TitleUI.h"
#include "../GameManager.h"
#include "UISpriteText.h"
#include "UIImage.h"

CTitleUI::CTitleUI() :
	m_Show(false),
	m_Race(nullptr),
	m_Tool(nullptr),
	m_Mode7(nullptr),
	m_Exit(nullptr)
{
}

CTitleUI::~CTitleUI()
{
}

bool CTitleUI::Init()
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	m_TitleImg = CreateWidget<CUIImage>("Title Image");

	m_TitleImg->SetTexture("Title Image", TEXT("Menu/Title.bmp"));

	m_TitleImg->SetPos(Vector2(139.0f, rs.height * 10.0f / 100.0f));

	m_Race = CreateWidget<CUISpriteText>("Race");

	m_Race->SetText("RACE");

	m_Race->SetPos(Vector2(rs.width * 34.0f / 100.0f, rs.height * 60.0f / 100.0f));

	m_Tool = CreateWidget<CUISpriteText>("Tool");

	m_Tool->SetText("MAP TOOL");

	m_Tool->SetPos(Vector2(rs.width * 34.0f / 100.0f, rs.height * 64.0f / 100.0f));

	m_Mode7 = CreateWidget<CUISpriteText>("Mode7");

	m_Mode7->SetText("MODE7 DEMO");

	m_Mode7->SetPos(Vector2(rs.width * 34.0f / 100.0f, rs.height * 68.0f / 100.0f));

	m_Exit = CreateWidget<CUISpriteText>("Exit");

	m_Exit->SetText("EXIT");

	m_Exit->SetPos(Vector2(rs.width * 34.0f / 100.0f, rs.height * 72.0f / 100.0f));


	return true;
}

void CTitleUI::Update(float deltaTime)
{
}

void CTitleUI::Render(HDC hDC)
{	
	m_TitleImg->Render(hDC);

	if (m_Show)
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();
		RECT rc;

		rc.left = 139;
		rc.top = rs.height * 55 / (float)100;
		rc.right = rc.left + m_TitleImg->GetSize().x;
		rc.bottom = rs.height * 80 / 100;

		FillRect(hDC, &rc, CGameManager::GetInst()->GetBlackBrush());

		m_Race->Render(hDC);
		m_Tool->Render(hDC);
		m_Mode7->Render(hDC);
		m_Exit->Render(hDC);
	}	
}

Vector2 CTitleUI::GetRaceTextPos()
{
	return m_Race->GetPos();
}

Vector2 CTitleUI::GetToolTextPos()
{
	return m_Tool->GetPos();
}

Vector2 CTitleUI::GetMode7TextPos()
{
	return m_Mode7->GetPos();
}

Vector2 CTitleUI::GetExitTextPos()
{
	return m_Exit->GetPos();
}
