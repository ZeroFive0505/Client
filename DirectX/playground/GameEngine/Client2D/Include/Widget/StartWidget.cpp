#include "StartWidget.h"
#include "Engine.h"
#include "../Scene/LoadingScene.h"
#include "Scene/SceneManager.h"

CStartWidget::CStartWidget()
{
}

CStartWidget::CStartWidget(const CStartWidget& window) :
    CWidgetWindow(window)
{
    m_StartButton = FindWidget<CButton>("StartButton");
    m_ExitButton = FindWidget<CButton>("ExitButton");
    m_Background = FindWidget<CImage>("Background");
}

CStartWidget::~CStartWidget()
{
}

void CStartWidget::Start()
{
}

bool CStartWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    sResolution rs = CEngine::GetInst()->GetResolution();

    SetSize((float)rs.width, (float)rs.height);

    m_StartButton = CreateWidget<CButton>("StartButton");
    m_ExitButton = CreateWidget<CButton>("ExitButton");
    m_Background = CreateWidget<CImage>("Background");

    m_StartButton->SetZOrder(1);
    m_ExitButton->SetZOrder(1);

    m_Background->SetTexture("StartBack", TEXT("Teemo.jpg"));
    m_Background->SetSize((float)rs.width, (float)rs.height);

    m_StartButton->SetPos((float)rs.width / 2.0f - 100.0f, (float)rs.height / 2.0f + 50.0f);
    m_StartButton->SetSize(200.0f, 50.0f);
    m_StartButton->SetTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
    m_StartButton->SetTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
    m_StartButton->SetTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

    m_StartButton->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
    m_StartButton->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
    m_StartButton->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

    // m_StartButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
    // m_StartButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "TeemoStartClicck.mp3");

    m_StartButton->SetClickCallback<CStartWidget>(this, &CStartWidget::OnStartButtonClick);

    m_ExitButton->SetPos((float)rs.width / 2.0f - 100.0f, (float)rs.height / 2.0f - 100.0f);
    m_ExitButton->SetSize(200.0f, 50.0f);
    m_ExitButton->SetTexture(Button_State::Normal, "EndButton", TEXT("End.png"));
    m_ExitButton->SetTexture(Button_State::MouseOn, "EndButton", TEXT("End.png"));
    m_ExitButton->SetTexture(Button_State::Click, "EndButton", TEXT("End.png"));

    m_ExitButton->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
    m_ExitButton->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
    m_ExitButton->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

    // m_ExitButton->SetSound(Button_Sound_State::MouseOn, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
    // m_ExitButton->SetSound(Button_Sound_State::Click, "UI", "ButtonClick", "TeemoStartClicck.mp3");

    m_ExitButton->SetClickCallback<CStartWidget>(this, &CStartWidget::OnExitButtonClick);

    return true;
}

void CStartWidget::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);
}

void CStartWidget::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CStartWidget::Render()
{
    CWidgetWindow::Render();
}

CStartWidget* CStartWidget::Clone()
{
    return new CStartWidget(*this);
}

void CStartWidget::OnStartButtonClick()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneMode<CLoadingScene>(false);
}

void CStartWidget::OnExitButtonClick()
{
    CEngine::GetInst()->Exit();
}
