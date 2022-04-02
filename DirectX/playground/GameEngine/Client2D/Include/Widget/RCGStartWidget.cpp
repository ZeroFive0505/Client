#include "RCGStartWidget.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../Scene/CRCGLoadingScene.h"
#include "../ClientManager.h"
#include "Input.h"

CRCGStartWidget::CRCGStartWidget()
{
}

CRCGStartWidget::CRCGStartWidget(const CRCGStartWidget& widget) :
    CWidgetWindow(widget)
{
    m_BackGround = FindWidget<CImage>("RCGStartBG");
    m_Border = FindWidget<CImage>("RCGStartBorder");
}

CRCGStartWidget::~CRCGStartWidget()
{
    CInput::GetInst()->ClearCallback();
}

void CRCGStartWidget::Start()
{
}

bool CRCGStartWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    sResolution rs = CEngine::GetInst()->GetResolution();

    SetSize((float)rs.width, (float)rs.height);

    m_BackGround = CreateWidget<CImage>("RCGStartBG");
    m_Border = CreateWidget<CImage>("RCGStartBorder");

    m_BackGround->SetZOrder(1);
    m_Border->SetZOrder(2);

    m_BackGround->SetTexture("RCGStartBack", TEXT("RCG_icon_1920x1080.png"));
    m_BackGround->SetSize((float)rs.width, (float)rs.height);

    m_Border->SetTexture("RCGStartBorderBack", TEXT("UI_ModeSelect_001_LETTERBOX.png"));
    m_Border->SetSize((float)rs.width, (float)rs.height);

    CInput::GetInst()->SetCallback<CRCGStartWidget>("Enter", KeyState_Down, this, &CRCGStartWidget::StartKeyDown);
    CInput::GetInst()->SetCallback<CRCGStartWidget>("Exit", KeyState_Down, this, &CRCGStartWidget::ExitKeyDown);

    return true;
}

void CRCGStartWidget::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);
}

void CRCGStartWidget::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CRCGStartWidget::Render()
{
    CWidgetWindow::Render();
}

CRCGStartWidget* CRCGStartWidget::Clone()
{
    return new CRCGStartWidget(*this);
}

void CRCGStartWidget::StartKeyDown(float deltaTime)
{
    CClientManager::GetInst()->GetManager()->SetHealth(160);
    CClientManager::GetInst()->GetManager()->SetSpecialGuage(100);

    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneMode<CRCGLoadingScene>(false);
}

void CRCGStartWidget::ExitKeyDown(float deltaTime)
{
    CEngine::GetInst()->Exit();
}
