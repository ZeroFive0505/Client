#include "ProgressBar.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"

CProgressBar::CProgressBar() :
    m_Percent(1.0f),
    m_Direction(ProgressBar_Direction::RightToLeft),
    m_ProgressCBuffer(nullptr),
    m_StartMouseOn(false)
{
    m_MouseCollisionEnable = true;
}

CProgressBar::CProgressBar(const CProgressBar& widget) :
    CWidget(widget)
{
    m_StartMouseOn = false;
    m_MouseOnCallback = nullptr;
    m_Percent = widget.m_Percent;
    m_Direction = widget.m_Direction;

    m_ProgressCBuffer = new CProgressBarConstantBuffer;

    m_ProgressCBuffer->Init();

    m_ProgressCBuffer->SetPercent(m_Percent);
    m_ProgressCBuffer->SetDirection(m_Direction);
}

CProgressBar::~CProgressBar()
{
    SAFE_DELETE(m_ProgressCBuffer);
}

bool CProgressBar::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (m_Owner->GetViewport())
    {
        if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, fileName, pathName))
            return false;

        m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
            return false;

        m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
    }

    SetUseTexture(true);

    return true;
}

bool CProgressBar::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    if (m_Owner->GetViewport())
    {
        if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, fullPath))
            return false;

        m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if(!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
            return false;

        m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
    }

    SetUseTexture(true);

    return true;
}

void CProgressBar::AddFrameData(const Vector2& start, const Vector2& size)
{
    sAnimationFrameData data;
    data.start = start;
    data.size = size;

    m_Info.vecFrameData.push_back(data);
}

void CProgressBar::Start()
{
    CWidget::Start();
}

bool CProgressBar::Init()
{
    if (!CWidget::Init())
        return false;

    if (m_Owner->GetViewport())
        m_Shader = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindShader("ProgressBarShader");
    else
        m_Shader = CResourceManager::GetInst()->FindShader("ProgressBarShader");

    m_ProgressCBuffer = new CProgressBarConstantBuffer;

    m_ProgressCBuffer->Init();

    m_ProgressCBuffer->SetPercent(m_Percent);
    m_ProgressCBuffer->SetDirection(m_Direction);

    return true;
}

void CProgressBar::Update(float deltaTime)
{
    CWidget::Update(deltaTime);

    if (m_MouseHovered && !m_StartMouseOn)
    {
        m_StartMouseOn = true;
        if (m_MouseOnCallback)
            m_MouseOnCallback();
    }
    else if (!m_MouseHovered && m_StartMouseOn)
    {
        m_StartMouseOn = false;
    }
}

void CProgressBar::PostUpdate(float deltaTime)
{
    CWidget::PostUpdate(deltaTime);
}

void CProgressBar::Render()
{
    if (m_Info.texture)
        m_Info.texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, 0);

    m_Tint = m_Info.tint;

    m_ProgressCBuffer->UpdateCBuffer();

    CWidget::Render();
}

CProgressBar* CProgressBar::Clone()
{
    return new CProgressBar(*this);
}
