#include "LoadingWidget.h"
#include "Engine.h"
#include "../Scene/MainScene.h"
#include "Scene/SceneManager.h"

CLoadingWidget::CLoadingWidget()
{
}

CLoadingWidget::CLoadingWidget(const CLoadingWidget& window) :
    CWidgetWindow(window)
{
    m_Background = FindWidget<CImage>("Back");
    m_LoadingBar = FindWidget<CProgressBar>("LoadingBar");
}

CLoadingWidget::~CLoadingWidget()
{
}

void CLoadingWidget::Start()
{
}

bool CLoadingWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    sResolution rs = CEngine::GetInst()->GetResolution();

    SetSize((float)rs.width, (float)rs.height);

    m_Background = CreateWidget<CImage>("Back");
    m_LoadingBar = CreateWidget<CProgressBar>("LoadingBar");

    m_Background->SetTexture("LoadingBack", TEXT("LoadingBack.jpg"));
    m_Background->SetSize((float)rs.width, (float)rs.height);

    m_LoadingBar->SetPos(150.0f, 100.0f);
    m_LoadingBar->SetSize(980.0f, 40.0f);
    m_LoadingBar->SetTexture("LoadingBar", TEXT("HPBar.png"));
    m_LoadingBar->SetZOrder(1);
    m_LoadingBar->SetPercent(0.0f);

    return true;
}

void CLoadingWidget::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);
}

void CLoadingWidget::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CLoadingWidget::Render()
{
    CWidgetWindow::Render();
}

CLoadingWidget* CLoadingWidget::Clone()
{
    return new CLoadingWidget(*this);
}
