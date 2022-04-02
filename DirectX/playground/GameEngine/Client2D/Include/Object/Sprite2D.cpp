#include "Sprite2D.h"

CSprite2D::CSprite2D()
{

}

CSprite2D::CSprite2D(const CSprite2D& obj) :
    CGameObject(obj)
{
    m_Sprite = (CSpriteComponent*)FindComponent("PlayerChildSprite");
}

CSprite2D::~CSprite2D()
{
}

bool CSprite2D::Init()
{
    m_Sprite = CreateComponent<CSpriteComponent>("PayerChildSprite");

    SetRootComponent(m_Sprite);

    m_Sprite->SetRelativeScale(50.0f, 50.0f, 1.0f);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

    return true;
}

void CSprite2D::Start()
{
    CGameObject::Start();
}

void CSprite2D::Update(float deltaTime)
{
}

void CSprite2D::PostUpdate(float deltaTime)
{
}

CSprite2D* CSprite2D::Clone()
{
    return new CSprite2D(*this);
}
