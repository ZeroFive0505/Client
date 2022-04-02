#include "SimpleHUD.h"

CSimpleHUD::CSimpleHUD()
{
}

CSimpleHUD::CSimpleHUD(const CSimpleHUD& window) :
	CWidgetWindow(window)
{
	m_NameText = FindWidget<CText>("NameText");
	m_HPBar = FindWidget<CProgressBar>("HPBar");
}

CSimpleHUD::~CSimpleHUD()
{
}

void CSimpleHUD::SetText(const TCHAR* text)
{
	m_NameText->SetText(text);
}

void CSimpleHUD::SetHPPercent(float percent)
{
	m_HPBar->SetPercent(percent);
}

void CSimpleHUD::SetHPDirection(ProgressBar_Direction direction)
{
	m_HPBar->SetDirection(direction);
}

void CSimpleHUD::Start()
{
}

bool CSimpleHUD::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(100.0f, 50.0f);

	m_NameText = CreateWidget<CText>("NameText");

	m_NameText->SetText(TEXT("Name"));
	m_NameText->SetPos(0.0f, 20.0f);
	m_NameText->SetSize(100.0f, 30.0f);
	m_NameText->SetFontSize(13.0f);
	m_NameText->SetZOrder(1);
	m_NameText->SetColor(1.0f, 1.0f, 1.0f);
	m_NameText->SetAlignH(TEXT_ALIGN_H::Center);

	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	// m_HPBar->SetDirection(ProgressBar_Direction::TopToBottom);
	m_HPBar->SetZOrder(2);
	m_HPBar->SetSize(100.0f, 20.0f);
	m_HPBar->SetTextureTint(255, 0, 0, 255);

	return true;
}

void CSimpleHUD::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CSimpleHUD::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CSimpleHUD::Render()
{
	CWidgetWindow::Render();
}

CSimpleHUD* CSimpleHUD::Clone()
{
	return new CSimpleHUD(*this);
}
