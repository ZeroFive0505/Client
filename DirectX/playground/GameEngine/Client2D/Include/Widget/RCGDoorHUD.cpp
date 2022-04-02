#include "RCGDoorHUD.h"

CRCGDoorHUD::CRCGDoorHUD()
{
}

CRCGDoorHUD::CRCGDoorHUD(const CRCGDoorHUD& window) :
    CWidgetWindow(window)
{
    m_Progress = FindWidget<CImage>("Door Progress");
    m_BackGround = FindWidget<CImage>("Door ProgressBG");
}

CRCGDoorHUD::~CRCGDoorHUD()
{
}

void CRCGDoorHUD::Start()
{
}

bool CRCGDoorHUD::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(100.0f, 100.0f);

    m_Progress = CreateWidget<CImage>("Door Progress");
    m_Progress->SetZOrder(2);
    m_Progress->SetSize(100.0f, 100.0f);
    m_Progress->SetPos(-50.0f, -50.0f);

    m_Progress->SetTexture("Door", TEXT("UI_UNLOCKED_DOOR.png"));
    m_Progress->SetTextureTint(255, 255, 255, 255);;

    m_BackGround = CreateWidget<CImage>("Door ProgressBG");

    m_BackGround->SetTexture("Door BG", TEXT("UI_UNLOCKED_DOOR.png"));
    m_BackGround->SetTextureTint(255, 255, 255, 255);
    m_BackGround->SetSize(100.0f, 100.0f);
    m_BackGround->SetOpacity(0.5f);
    m_BackGround->SetPos(-50.0f, -50.0f);

    return true;
}

void CRCGDoorHUD::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);
}

void CRCGDoorHUD::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CRCGDoorHUD::Render()
{
    CWidgetWindow::Render();
}

CRCGDoorHUD* CRCGDoorHUD::Clone()
{
    return new CRCGDoorHUD(*this);
}
