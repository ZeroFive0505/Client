#include "Image.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

CImage::CImage()
{
}

CImage::CImage(const CImage& widget) :
    CWidget(widget)
{
    m_ClickCallback = nullptr;
    m_PlayOnce = false;
    m_Loop = true;
    m_Render = true;
}

CImage::~CImage()
{
}

bool CImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (m_Owner->GetViewport())
    {
        if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, fileName, pathName))
            return false;

        m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if (CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
            return false;

        m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
    }

    SetUseTexture(true);

    return true;
}

bool CImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    if (m_Owner->GetViewport())
    {
        if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, fullPath))
            return false;

        m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
            return false;

        m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
    }


    SetUseTexture(true);

    return true;
}

bool CImage::SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
    if (m_Owner->GetViewport())
    {
        if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, vecFileName, pathName))
            return false;

        m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
            return false;

        m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
    }

    SetUseTexture(true);

    return true;
}

bool CImage::SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
    if (m_Owner->GetViewport())
    {
        if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, vecFullPath))
            return false;

        m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
            return false;

        m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
    }

    SetUseTexture(true);

    return true;
}

void CImage::AddFrameData(const Vector2& start, const Vector2& size)
{
    sAnimationFrameData data;
    data.start = start;
    data.size = size;

    m_Info.vecFrameData.push_back(data);
}

void CImage::AddFrameData(int count)
{
    if (m_Info.texture->GetImageType() == Image_Type::Atlas)
        return;

    for (int i = 0; i < count; i++)
    {
        sAnimationFrameData data;
        data.start = Vector2(0.0f, 0.0f);
        data.size = Vector2((float)m_Info.texture->GetWidth(i), (float)m_Info.texture->GetHeight(i));

        m_Info.vecFrameData.push_back(data);
    }
}

void CImage::Start()
{
    CWidget::Start();
}

bool CImage::Init()
{
    if (!CWidget::Init())
        return false;

    m_PlayOnce = false;
    m_Loop = true;
    m_Render = true;
    m_AnimStart = true;
    
    return true;
}

void CImage::Update(float deltaTime)
{
    CWidget::Update(deltaTime);
}

void CImage::PostUpdate(float deltaTime)
{
    CWidget::PostUpdate(deltaTime);

    if (!m_AnimStart)
        return;

    if (m_Loop)
    {
        if (!m_Info.vecFrameData.empty())
        {
            m_Info.elapsedTime += deltaTime * m_Info.playScale;

            m_Info.frameTime = m_Info.playTime / m_Info.vecFrameData.size();

            if (m_Info.elapsedTime >= m_Info.frameTime)
            {
                m_Info.elapsedTime -= m_Info.frameTime;

                m_Info.frame++;

                if (m_Info.frame == m_Info.vecFrameData.size() && m_PlayOnce)
                {
                    m_Render = false;
                    m_Loop = false;
                }
                else
                    m_Info.frame %= m_Info.vecFrameData.size();
            }
        }
    }
}

void CImage::Render()
{
    if (!m_Render)
        return;

    int frame = 0;

    if (!m_Info.vecFrameData.empty() && m_Loop)
    {
        m_CBuffer->SetAnimEnable(true);
        m_CBuffer->SetAnimType((int)m_Info.texture->GetImageType());

        switch (m_Info.texture->GetImageType())
        {
        case Image_Type::Atlas:
        {
            Vector2 startUV, endUV;

            Vector2 start = m_Info.vecFrameData[m_Info.frame].start;
            Vector2 frameSize = m_Info.vecFrameData[m_Info.frame].size;

            startUV = start / Vector2((float)m_Info.texture->GetWidth(), (float)m_Info.texture->GetHeight());

            endUV = (start + frameSize) / Vector2((float)m_Info.texture->GetWidth(), (float)m_Info.texture->GetHeight());

            m_CBuffer->SetStartUV(startUV);
            m_CBuffer->SetEndUV(endUV);
        }
        break;
        case Image_Type::Frame:
            frame = m_Info.frame;

            m_CBuffer->SetStartUV(Vector2(0.0f, 0.0f));
            m_CBuffer->SetEndUV(Vector2(1.0f, 1.0f));

            break;
        case Image_Type::Array:
            break;
        }
    }
    else
        m_CBuffer->SetAnimEnable(false);


    if (m_Info.texture)
        m_Info.texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, frame);

    m_Tint = m_Info.tint;

    CWidget::Render();
}

CImage* CImage::Clone()
{
    return new CImage(*this);
}
