#include "RCGBossWidget.h"
#include "Scene/SceneManager.h"

CRCGBossWidget::CRCGBossWidget()
{
    SetTypeID<CRCGBossWidget>();
}

CRCGBossWidget::CRCGBossWidget(const CRCGBossWidget& window)
{
    m_BossHealthBarBack = FindWidget<CImage>("BossHPBack");
    m_BossHealth = FindWidget<CProgressBar>("BossHealth");
}

CRCGBossWidget::~CRCGBossWidget()
{
}

void CRCGBossWidget::Start()
{
    CWidgetWindow::Start();
}

bool CRCGBossWidget::Init()
{
    sResolution rs = CEngine::GetInst()->GetResolution();

    SetSize((float)rs.width, (float)rs.height);

    m_BossHealthBarBack = CreateWidget<CImage>("BossHPBack");
    m_BossHealth = CreateWidget<CProgressBar>("BossHealth");

    SetZOrder(2);

    m_BossHealthBarBack->SetZOrder(2);
    m_BossHealth->SetZOrder(1);


    m_BossHealthBarBack->SetTexture("BossHPBack", TEXT("RCG_bossmeter_frame.png"));
    Vector2 backSize = m_BossHealthBarBack->GetTextureSize();
    m_BossHealthBarBack->SetSize(backSize.x * 0.75f, backSize.y);
    m_BossHealthBarBack->SetCollisionEnable(false);
    m_BossHealthBarBack->SetOpacity(0.0f);
    m_BossHealthBarBack->SetPos((float)rs.width * 0.25f, 30.0f);

    m_BossHealth->SetTexture("BossHP", TEXT("RCG_finalbossmeter_fill.png"));
    Vector2 size = m_BossHealth->GetTextureSize();
    m_BossHealth->SetSize(size.x * 0.75f - 190.0f, 80.0f);
    m_BossHealth->SetCollisionEnable(false);
    m_BossHealth->SetOpacity(0.0f);
    m_BossHealth->SetPercent(1.0f);
    m_BossHealth->SetPos((float)rs.width * 0.25f + 20.0f, 40.0f);

    m_FadeIn = false;

    m_FadeOut = false;

    return true;
}

void CRCGBossWidget::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);

    if (m_FadeIn)
    {
        m_FadeTime += deltaTime;

        if (m_FadeTime > 1.0f)
        {
            m_FadeIn = false;
            m_FadeTime = 1.0f;
        }

        m_BossHealth->SetOpacity(m_FadeTime);
        m_BossHealthBarBack->SetOpacity(m_FadeTime);
    }
}

void CRCGBossWidget::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CRCGBossWidget::Render()
{
    CWidgetWindow::Render();
}
