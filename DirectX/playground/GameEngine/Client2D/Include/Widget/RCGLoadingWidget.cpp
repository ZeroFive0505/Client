#include "RCGLoadingWidget.h"
#include "Engine.h"
#include "../Scene/MainScene.h"
#include "Scene/SceneManager.h"

CRCGLoadingWidget::CRCGLoadingWidget()
{
}

CRCGLoadingWidget::CRCGLoadingWidget(const CRCGLoadingWidget& window) :
    CWidgetWindow(window)
{
    m_BackGround = FindWidget<CImage>("Loading back");
    m_LoadingAnim = FindWidget<CImage>("Loading Anim");
}

CRCGLoadingWidget::~CRCGLoadingWidget()
{
}

void CRCGLoadingWidget::Start()
{
}

bool CRCGLoadingWidget::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    sResolution rs = CEngine::GetInst()->GetResolution();

    SetSize((float)rs.width, (float)rs.height);

    m_BackGround = CreateWidget<CImage>("Loading Back");
    m_BackGround->SetTexture("Loading Back", TEXT("Background.png"));
    m_BackGround->SetZOrder(1);
    m_LoadingAnim = CreateWidget<CImage>("Loading Anim");
    m_LoadingAnim->SetZOrder(2);

    m_BackGround->SetTextureTint(0, 0, 0, 255);
    m_BackGround->SetSize((float)rs.width, (float)rs.height);

	std::vector<TCHAR*> vecFileName;

    for (int i = 1; i <= 4; i++)
    {
        TCHAR* fileName = new TCHAR[MAX_PATH];

        memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(fileName, TEXT("RCG_punchload_0%d.png"), i);

        vecFileName.push_back(fileName);
    }

    m_LoadingAnim->SetPlayScale(1.0f);

    m_LoadingAnim->SetSize(200.0f, 150.0f);

    m_LoadingAnim->SetTexture("Loading Animation", vecFileName);

    m_LoadingAnim->SetPos((float)rs.width - 200.0f, 0.0f);

    m_LoadingAnim->AddFrameData(4);

    size_t size = vecFileName.size();

    for (size_t i = 0; i < size; i++)
    {
        SAFE_DELETE(vecFileName[i]);
    }

    return true;
}

void CRCGLoadingWidget::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);
}

void CRCGLoadingWidget::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CRCGLoadingWidget::Render()
{
    CWidgetWindow::Render();
}

CRCGLoadingWidget* CRCGLoadingWidget::Clone()
{
    return new CRCGLoadingWidget(*this);
}
