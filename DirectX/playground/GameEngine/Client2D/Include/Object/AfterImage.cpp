#include "AfterImage.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/SceneResource.h"

CAfterImage::CAfterImage() :
    m_Transform(nullptr),
    m_Size(0),
    m_Index(0),
    m_EffectOn(false)
{
    SetTypeID<CAfterImage>();
    m_Opacity = 0.5f;
}

CAfterImage::CAfterImage(const CAfterImage& obj) :
    CGameObject(obj)
{
    m_Transform = (CSceneComponent*)FindComponent("AfterImage Transform");
}

CAfterImage::~CAfterImage()
{
}

bool CAfterImage::Init()
{
    m_Transform = CreateComponent<CSceneComponent>("AfterImage Transform");

    m_Transform->SetPivot(0.5f, 0.0f, 0.0f);

    m_Size = 15;

    m_vecSprites.resize(m_Size);

    size_t size = m_vecSprites.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecSprites[i] = CreateComponent<CSpriteComponent>("AfterImage Sprite " + std::to_string(i + 1));
        m_vecSprites[i]->SetPivot(0.5f, 0.0f, 0.0f);
        m_vecSprites[i]->SetTransparency(true);
        m_vecSprites[i]->SetLayerName("Shadow");
        m_vecSprites[i]->SetWorldPos(9999.0f, 9999.0f, 0.0f);
        m_Transform->AddChild(m_vecSprites[i]);
    }

    return true;
}

void CAfterImage::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);

    size_t size = m_vecSprites.size();

    for (size_t i = 0; i < size; i++)
    {
        int currentFrame = m_vecSprites[i]->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

        m_vecSprites[i]->SetRelativeScale(m_vecSprites[i]->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size.x * 2.0f,
            m_vecSprites[i]->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size.y * 2.0f, 1.0f);

        Vector4 color = m_vecSprites[i]->GetBaseColor();

        if (color.w > 0.0f)
        {
            if (!m_OffsetOn)
                color.w -= deltaTime;
            else
                color.w -= deltaTime * 30.0f / ((int)i + 1);
        }

        color.w = fmaxf(0.0f, color.w);
        m_vecSprites[i]->SetOpacity(color.w);

        m_vecSprites[i]->SetBaseColor(color);

        if (color.w <= 0.0f)
            m_vecSprites[i]->SetWorldPos(9999.0f, 9999.0f, 0.0f);
    }
}

void CAfterImage::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CAfterImage* CAfterImage::Clone()
{
    return new CAfterImage(*this);
}

void CAfterImage::SetAnimation(const std::string name)
{
    size_t size = m_vecSprites.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecSprites[i]->ChangeAnimation(name);
    }
}

void CAfterImage::SetFrame(int frame)
{
    m_Index = m_Index % (int)m_vecSprites.size();

    m_vecSprites[m_Index]->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(frame);

    m_vecSprites[m_Index]->SetBaseColor(m_TintColor);
}

void CAfterImage::SetPos(const Vector3& pos)
{
    m_Index = m_Index % (int)m_vecSprites.size();

    if(!m_OffsetOn)
        m_vecSprites[m_Index]->SetWorldPos(pos);
    else
    {
        Vector3 offsetPos = pos;

        offsetPos += (m_Offset * m_Index);

        m_vecSprites[m_Index]->SetWorldPos(offsetPos);
    }
}

void CAfterImage::Prepare()
{
    size_t size = m_vecSprites.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecSprites[i]->SetWorldPos(9999.0f, 9999.0f, 0.0f);
    }
}

void CAfterImage::SetTintColor(const Vector4& color)
{
    size_t size = m_vecSprites.size();

    m_TintColor = color;

    for (size_t i = 0; i < size; i++)
    {
        m_vecSprites[i]->SetBaseColor(color);
    }
}

void CAfterImage::FlipImage(bool flip)
{
    size_t size = m_vecSprites.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecSprites[i]->GetAnimationInstance()->SetHorizontalFlip(flip);
    }
}
