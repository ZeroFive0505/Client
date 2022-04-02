#include "RaceMenuUI.h"
#include "UISpriteText.h"
#include "../GameManager.h"


CRaceMenuUI::CRaceMenuUI() :
	m_CC50(nullptr),
	m_CC100(nullptr),
	m_CC150(nullptr),
	m_CC200(nullptr)
{
}

CRaceMenuUI::~CRaceMenuUI()
{
}

bool CRaceMenuUI::Init()
{
	Resolution rs = CGameManager::GetInst()->GetResolution();

	m_Visibility = false;

	m_CC50 = CreateWidget<CUISpriteText>("50CC");

	m_CC50->SetText("50CC");

	m_CC50->SetPos(Vector2(rs.width * 40.0f / 100.0f, rs.height * 50.0f / 100.0f));

	m_CC100 = CreateWidget<CUISpriteText>("100CC");

	m_CC100->SetText("100CC");

	m_CC100->SetPos(Vector2(rs.width * 40.0f / 100.0f, rs.height * 55.0f / 100.0f));

	m_CC150 = CreateWidget<CUISpriteText>("150CC");

	m_CC150->SetText("150CC");

	m_CC150->SetPos(Vector2(rs.width * 40.0f / 100.0f, rs.height * 60.0f / 100.0f));

	m_CC200 = CreateWidget<CUISpriteText>("200CC");
	
	m_CC200->SetText("200CC");

	m_CC200->SetPos(Vector2(rs.width * 40.0f / 100.0f, rs.height * 65.0f / 100.0f));



	return true;
}

void CRaceMenuUI::Update(float deltaTime)
{
}

void CRaceMenuUI::Render(HDC hDC)
{
	/*
	* Resolution rs = CGameManager::GetInst()->GetResolution();
		RECT rc;

		rc.left = 139.0f;
		rc.top = rs.height * 55 / 100;
		rc.right = rc.left + m_TitleImg->GetSize().x;
		rc.bottom = rs.height * 80 / 100;

		FillRect(hDC, &rc, CGameManager::GetInst()->GetBlackBrush());
	*/

	Resolution rs = CGameManager::GetInst()->GetResolution();

	RECT rc;

	rc.left = 170;
	rc.top = rs.height * 50 / (float)100;
	rc.right = rc.left + 150;
	rc.bottom = rs.height * 80 / (float)100;

	FillRect(hDC, &rc, CGameManager::GetInst()->GetBlackBrush());

	m_CC50->Render(hDC);
	m_CC100->Render(hDC);
	m_CC150->Render(hDC);
	m_CC200->Render(hDC);
}

Vector2 CRaceMenuUI::GetCC50Pos() const
{
	return m_CC50->GetPos();
}

Vector2 CRaceMenuUI::GetCC100Pos() const
{
	return m_CC100->GetPos();
}

Vector2 CRaceMenuUI::GetCC150Pos() const
{
	return m_CC150->GetPos();
}

Vector2 CRaceMenuUI::GetCC200Pos() const
{
	return m_CC200->GetPos();
}
