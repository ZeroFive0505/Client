#include "BulletTornado.h"
#include "Component/SpriteComponent.h"

CBulletTornado::CBulletTornado()
{
    SetTypeID<CBulletTornado>();
}

CBulletTornado::CBulletTornado(const CBulletTornado& obj) :
    CGameObject(obj)
{
    m_Root = (CSceneComponent*)FindComponent("Root");
    m_Root1 = (CSceneComponent*)FindComponent("Root1");
    m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
}

CBulletTornado::~CBulletTornado()
{
}

bool CBulletTornado::Init()
{
    m_Root = CreateComponent<CSceneComponent>("Root");
    m_Root1 = CreateComponent<CSceneComponent>("Root1");
    m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");


    SetRootComponent(m_Root);

    m_Root->AddChild(m_Root1);
    m_Root1->AddChild(m_Sprite);

    m_Sprite->SetRelativeScale(50.0f, 50.0f, 1.0f);
    m_Sprite->SetRelativePos(50.0f, 0.0f, 0.0f);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);
    m_Sprite->SetInheritRotZ(true);

    return true;
}

void CBulletTornado::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);

    AddRelativePos(GetWorldAxis(AXIS_Y) * 500.0f * deltaTime);

    m_Root1->AddRelativeRotationZ(720.0f * deltaTime);
}

void CBulletTornado::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CBulletTornado* CBulletTornado::Clone()
{
    return new CBulletTornado(*this);
}
