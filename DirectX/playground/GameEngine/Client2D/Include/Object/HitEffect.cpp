#include "HitEffect.h"
#include "HitAnimation.h"

CHitEffect::CHitEffect()
{
    SetTypeID<CHitEffect>();
    m_Opacity = 0.5f;
}

CHitEffect::CHitEffect(const CHitEffect& obj) :
    CGameObject(obj)
{
    m_Sprite = (CSpriteComponent*)FindComponent("Hit Effect");
}

CHitEffect::~CHitEffect()
{
}

bool CHitEffect::Init()
{
    m_Sprite = CreateComponent<CSpriteComponent>("Hit Effect");
    
    m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

    m_Sprite->CreateAnimationInstance<CHitAnimation>();

    m_Sprite->GetAnimationInstance()->SetEndFunction<CHitEffect>("HitVFX", this, &CHitEffect::Destroy);

    m_Sprite->SetLayerName("Particle");

    m_Sprite->SetTransparency(true);

    m_Sprite->SetWorldScale(128.0f, 128.0f, 1.0f);

    return true;
}

void CHitEffect::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CHitEffect::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CHitEffect* CHitEffect::Clone()
{
    return new CHitEffect(*this);
}
