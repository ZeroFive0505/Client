#include "GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CGameObject::CGameObject() :
    m_Scene(nullptr),
    m_Start(false),
    m_ImageStart(0, 0),
    m_PrevPos(0, 0),
    m_Pos(0, 0),
    m_Size(0, 0),
    m_Pivot(0, 0),
    m_Offset(0, 0),
    m_Angle(0),
    m_Animation(nullptr),
    m_MoveSpeed(1.0f)
{
}

CGameObject::CGameObject(const Vector2& position, const float visualRadius, 
    const float hitBoxRadius, const float height, const int mapWidth, const int mapHeight)
{
    float scaleX = 1.0f / mapWidth;
    float scaleY = 1.0f / mapHeight;
    m_Pos = Vector2(position.x * scaleX, position.y * scaleY);
    m_Angle = 0;
    m_PrevPos = Vector2(0, 0);
    m_ImageStart = Vector2(0, 0);
    m_Size = Vector2(0, 0);
    m_Offset = Vector2(0, 0);
}

CGameObject::CGameObject(const CGameObject& obj)
{
    m_Start = false;

    m_Scene = obj.m_Scene;

    m_PrevPos = obj.m_PrevPos;
    m_Pos = obj.m_Pos;
    m_Size = obj.m_Size;
    m_Offset = obj.m_Offset;
    m_ImageStart = obj.m_ImageStart;
}

CGameObject::~CGameObject()
{
    SAFE_DELETE(m_Animation);
}

float CGameObject::GetBottom() const
{
    Vector2 size = m_Size;

    return m_Pos.y + (1.0f - m_Pivot.y) * size.y + m_Offset.y;
}

void CGameObject::SetScene(CScene* scene)
{
    m_Scene = scene;
}

void CGameObject::Move(const Vector2& dir)
{
    Vector2 move = dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime();
    m_Pos += move;
}

void CGameObject::Move(const Vector2& dir, float speed)
{
    Vector2 move = dir * speed * CGameManager::GetInst()->GetDeltaTime();
    m_Pos += move;
}

void CGameObject::Start()
{
    m_Start = true;
}

bool CGameObject::Init()
{
    return true;
}

void CGameObject::Update(float deltaTime)
{
    if (!m_Start)
        Start();

    if (m_Animation)
        m_Animation->Update(deltaTime);

}

void CGameObject::PostUpdate(float deltaTime)
{
    if (m_Animation)
    {
        AnimationInfo* animInfo = m_Animation->m_CurrentAnimation;

        const AnimationFrameData& frameData = animInfo->sequence->GetFrameData(animInfo->frame);

        m_Size = frameData.size;
    }
}

void CGameObject::Collision(float deltaTime)
{
}


void CGameObject::Render(HDC hDC)
{
    if (m_Animation)
    {
        AnimationInfo* animInfo = m_Animation->m_CurrentAnimation;

        const AnimationFrameData& frameData = animInfo->sequence->GetFrameData(animInfo->frame);

        Vector2 LT = m_Pos - m_Pivot * frameData.size + m_Offset;

        if (animInfo->sequence->GetTextureType() == ETexture_Type::Atlas)
        {
            animInfo->sequence->GetTexture()->Render(hDC, LT, frameData.startPos, frameData.size);
        }
        else
        {
            animInfo->sequence->GetTexture()->Render(hDC, LT, frameData.startPos, frameData.size, animInfo->frame);
        }
    }
    else
    {
        Vector2 LT = m_Pos - m_Pivot * m_Size + m_Offset;

        if (m_Texture)
        {
            m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
        }
        else
        {
            Rectangle(hDC, (int)LT.x, (int)LT.y, (int)(LT.x + m_Size.x), (int)(LT.y + m_Size.y));
        }
    }
}

CGameObject* CGameObject::Clone()
{
    return nullptr;
}

void CGameObject::SetTexture(const std::string& name)
{
    m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CGameObject::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    m_Scene->GetSceneResoruce()->LoadTexture(name, fileName, pathName);

    m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CGameObject::SetTextureFullPath(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
    m_Scene->GetSceneResoruce()->LoadTexture(name, vecFileName, pathName);

    m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

    if (m_Texture)
    {
        m_Size.x = (float)m_Texture->GetWidth();
        m_Size.y = (float)m_Texture->GetHeight();
    }
}

void CGameObject::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int index)
{
    if (m_Texture)
        m_Texture->SetColorKey(r, g, b, index);
}

void CGameObject::CreateAnimation()
{
    if (!m_Animation)
    {
        m_Animation = new CAnimation;

        m_Animation->m_Owner = this;
        m_Animation->m_Scene = m_Scene;
    }
}

void CGameObject::AddAnimation(const std::string& sequenceName, bool loop, float playTime, float playScale, bool reverse)
{
    if (!m_Animation)
    {
        m_Animation = new CAnimation;

        m_Animation->m_Owner = this;
        m_Animation->m_Scene = m_Scene;
    }

    m_Animation->AddAnimation(sequenceName, loop, playTime, playScale, reverse);
}

void CGameObject::SetAnimationPlayTime(const std::string& name, float playTime)
{
    if (!m_Animation)
        return;

    m_Animation->SetPlayTime(name, playTime);
}

void CGameObject::SetAnimationPlayScale(const std::string& name, float playScale)
{
    if (!m_Animation)
        return;

    m_Animation->SetPlayScale(name, playScale);
}

void CGameObject::SetCurrentAnimation(const std::string& name)
{
    if (!m_Animation)
        return;

    m_Animation->SetCurrentAnimation(name);
}

void CGameObject::ChangeAnimation(const std::string& name)
{
    if (m_Animation)
        return;

    m_Animation->ChangeAnimation(name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& name)
{
    return m_Animation->CheckCurrentAnimation(name);
}

void CGameObject::SetAnimationReverse(const std::string& name, bool reverse)
{
    if (!m_Animation)
        return;

    m_Animation->SetReverse(name, reverse);
}

void CGameObject::SetAnimationLoop(const std::string& name, bool loop)
{
    if (!m_Animation)
        return;

    m_Animation->SetLoop(name, loop);
}
