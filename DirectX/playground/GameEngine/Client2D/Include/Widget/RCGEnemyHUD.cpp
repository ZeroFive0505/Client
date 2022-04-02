#include "RCGEnemyHUD.h"

CRCGEnemyHUD::CRCGEnemyHUD()
{
}

CRCGEnemyHUD::CRCGEnemyHUD(const CRCGEnemyHUD& window) :
    CWidgetWindow(window)
{
    m_ExclamationMark = FindWidget<CImage>("Exclamation Mark");
}

CRCGEnemyHUD::~CRCGEnemyHUD()
{
}

void CRCGEnemyHUD::Start()
{
}

bool CRCGEnemyHUD::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(45.0f, 89.0f);

    m_ExclamationMark = CreateWidget<CImage>("Exclamation Mark");
    m_ExclamationMark->SetTexture("ExclamationMark", TEXT("UI_EXCLAMATION-POINT.png"));

    m_ExclamationMark->SetRender(false);

    return true;
}

void CRCGEnemyHUD::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);

    m_ElapsedTime += deltaTime;

    if (m_ShowExclamation && m_ElapsedTime >= 2.0f)
    {
        m_ExclamationMark->SetRender(false);
        m_ShowExclamation = false;
    }
}

void CRCGEnemyHUD::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CRCGEnemyHUD::Render()
{
    CWidgetWindow::Render();
}

CRCGEnemyHUD* CRCGEnemyHUD::Clone()
{
    return new CRCGEnemyHUD(*this);
}
