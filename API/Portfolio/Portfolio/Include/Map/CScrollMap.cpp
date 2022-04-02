#include "CScrollMap.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CScrollMap::CScrollMap() :
    m_ScrollRatio(1.0f, 0.0f),
    m_Loop(true)
{
}

CScrollMap::~CScrollMap()
{
}

void CScrollMap::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    m_Scene->GetSceneResoruce()->LoadTexture(name, fileName, pathName);

    SetTexture(name);
}

void CScrollMap::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    m_Scene->GetSceneResoruce()->LoadTextureFullPath(name, fullPath);

    SetTexture(name);
}

void CScrollMap::SetTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
    m_Scene->GetSceneResoruce()->LoadTexture(name, vecFileName, pathName);

    SetTexture(name);
}

void CScrollMap::SetTexture(const std::string& name)
{
    m_ScrollTexture = m_Scene->GetSceneResoruce()->FindTexture(name);
}

void CScrollMap::SetTextureColorKey(const unsigned char r, const unsigned char g, const unsigned char b)
{
    if (m_ScrollTexture)
        m_ScrollTexture->SetColorKey(255, 0, 255);
}

void CScrollMap::Start()
{
    CMapBase::Start();

    if (m_ScrollTexture)
        m_TextureSize = Vector2((float)m_ScrollTexture->GetWidth(), (float)m_ScrollTexture->GetHeight());
}

bool CScrollMap::Init()
{
    CMapBase::Init();

    return true;
}

void CScrollMap::Update(float deltaTime)
{
    CMapBase::Update(deltaTime);
}

void CScrollMap::PostUpdate(float deltaTime)
{
    CMapBase::PostUpdate(deltaTime);
}

void CScrollMap::Render(HDC hDC)
{
    CMapBase::Render(hDC);

    if (m_ScrollTexture)
    {
        if (!m_Loop)
        {
            CCamera* camera = m_Scene->GetCamera();

            Vector2 resolution = camera->GetResolution();

            Vector2 imgPos = camera->GetPos() * m_ScrollRatio;

            unsigned int width = m_ScrollTexture->GetWidth();
            unsigned int height = m_ScrollTexture->GetHeight();

            imgPos.x = imgPos.x < 0.0f ? 0.0f : imgPos.x;
            imgPos.x = imgPos.x + resolution.x > width ? width - resolution.x : imgPos.x;

            imgPos.y = imgPos.y < 0.0f ? 0.0f : imgPos.y;
            imgPos.y = imgPos.y + resolution.y > height ? height - resolution.y : imgPos.y;

            m_ScrollTexture->Render(hDC, m_Pos, imgPos, m_Size);
        }
        else
        {
            CCamera* camera = m_Scene->GetCamera();

            Vector2 cameraPos = camera->GetPos();

            Vector2 resolution = camera->GetResolution();

            Vector2 imgPos = camera->GetPos() * m_ScrollRatio;

            unsigned int width = m_ScrollTexture->GetWidth();
            unsigned int height = m_ScrollTexture->GetHeight();

            unsigned int widthRatio = (unsigned int)(cameraPos.x / width);
            unsigned int heightRatio = (unsigned int)(cameraPos.y / height);

            Vector2 convertPos;

            convertPos.x = cameraPos.x - widthRatio * width;
            convertPos.y = cameraPos.y - heightRatio * height;

            Vector2 convertLTSize;
            convertLTSize.x = width - convertPos.x;
            convertLTSize.y = height - convertPos.y;

            m_ScrollTexture->Render(hDC, Vector2(0.0f, 0.0f), convertPos, convertLTSize);

            m_ScrollTexture->Render(hDC, Vector2(convertLTSize.x, 0.0f), Vector2(0.0f, height - convertLTSize.y),
                Vector2(resolution.x - convertLTSize.x, convertLTSize.y));

            m_ScrollTexture->Render(hDC, Vector2(0.0f, convertLTSize.y),
                Vector2(convertPos.x, 0.0f),
                Vector2(convertLTSize.x, resolution.y - convertLTSize.y));

            m_ScrollTexture->Render(hDC, Vector2(convertLTSize.x, convertLTSize.y),
                Vector2(0.0f, 0.0f),
                Vector2(resolution.x - convertLTSize.x, resolution.y - convertLTSize.y));
        }
    }
}

void CScrollMap::Save(FILE* pFile)
{
}

void CScrollMap::Load(FILE* pFile)
{
}
