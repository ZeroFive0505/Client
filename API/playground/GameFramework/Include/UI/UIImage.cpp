#include "UIImage.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CUIImage::CUIImage() :
    m_FrameIndex(0),
    m_PlayTime(1.0f),
    m_AnimTime(0.0f)
{
}

CUIImage::CUIImage(const CUIImage& widget) :
    CUIWidget(widget)
{
    m_Texture = widget.m_Texture;
    m_PlayTime = widget.m_PlayTime;
    m_AnimTime = 0.0f;
    m_FrameIndex = 0;
    m_vecFrameData = widget.m_vecFrameData;
}

CUIImage::~CUIImage()
{
}


void CUIImage::SetTexture(const std::string& Name)
{
    // 먼저 현재 씬 리소스에 텍스쳐가 있는지 찾는다.
    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
    // 씬 리소스를 통해 새로운 텍스쳐를 불러온다.
    m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

    // 불러온 텍스쳐를 다시 찾아온다.
    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
    // 텍스처를 전체 경로를 통해서 불러온다.
    m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

    // 불러온 텍스쳐를 찾아와서 할당한다.
    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CUIImage::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
    m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

    m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
    if (m_Texture)
        m_Texture->SetColorKey(r, g, b, Index);
}

bool CUIImage::Init()
{
    return true;
}

void CUIImage::Update(float DeltaTime)
{
    // 만약 애니메이션이 있다면
    if (!m_vecFrameData.empty())
    {
        // 재생시간을 늘린다.
        m_AnimTime += DeltaTime;

        float FrameTime = m_PlayTime / m_vecFrameData.size();

        // 다음 프레임으로 넘어가야할떄는
        if (m_AnimTime >= FrameTime)
        {
            // 재생시간을 초기화시키고
            m_AnimTime -= FrameTime;

            // 다음 인덱스로 넘긴다.
            m_FrameIndex = (m_FrameIndex + 1) % m_vecFrameData.size();
        }
    }
}

void CUIImage::PostUpdate(float DeltaTime)
{
}

void CUIImage::Collision(float DeltaTime)
{
}

void CUIImage::Render(HDC hDC)
{
    // 만약 출력될 텍스쳐가 있다면
    if (m_Texture)
    {
        // 이 UI를 가지고 있는 소요자의 위치를 가져온다.
        Vector2 Pos = m_Pos + m_Owner->GetPos();
        Vector2 ImagePos;
        Vector2 Size = m_Size;

        // 만약 애니메이션이 있다면
        if (!m_vecFrameData.empty())
        {
            // 이미지가 시작될 위치와 크기를 가져온다.
            ImagePos = m_vecFrameData[m_FrameIndex].StartPos;
            Size = m_vecFrameData[m_FrameIndex].Size;

            // 만약 이미지가 낱장으로 되어있다면 인덱스를 전달해서 출력한다.
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
            }
            // 아닐경우 하나의 큰 텍스쳐라는 뜻 따라서 인덱스는 전달하지 않는다.
            else
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
            }
        }
        else
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
            }
            else
            {
                m_Texture->Render(hDC, Pos + m_Offset, Vector2(0.0f, 0.0f), Size);
            }
        }
    }
}

void CUIImage::Render(const Vector2& Pos, HDC hDC)
{
    // Update와 동일하다. 
    if (m_Texture)
    {
        Vector2 ImagePos;
        Vector2 Size = m_Size;
        if (!m_vecFrameData.empty())
        {
            ImagePos = m_vecFrameData[m_FrameIndex].StartPos;
            Size = m_vecFrameData[m_FrameIndex].Size;

            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
            }
            else
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
            }
        }
        else
        {
            if (m_Texture->GetTextureType() == ETexture_Type::Frame)
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size, m_FrameIndex);
            }
            else
            {
                m_Texture->Render(hDC, Pos + m_Offset, ImagePos, Size);
            }
        }
    }
}

CUIImage* CUIImage::Clone()
{
    return new CUIImage(*this);
}
