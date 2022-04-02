#include "Attractor.h"
#include "Engine.h"
#include "Mover.h"
#include "Scene/Scene.h"

CAttractor::CAttractor()
{
    SetTypeID<CAttractor>();
}

CAttractor::CAttractor(const CAttractor& obj)
{
    m_Sprite = (CSpriteComponent*)FindComponent("AttractorSprite");
}

CAttractor::~CAttractor()
{
}

bool CAttractor::Init()
{
    CGameObject::Init();

    m_Sprite = CreateComponent<CSpriteComponent>("AttractorSprite");

    m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

    SetRootComponent(m_Sprite);

    sResolution rs = CEngine::GetInst()->GetResolution();

    m_ScreenSize.x = (float)rs.width;
    m_ScreenSize.y = (float)rs.height;

    m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Mover", TEXT("awesomeface.png"));

    m_Sprite->SetWorldPos(m_ScreenSize.x / 2.0f, m_ScreenSize.y / 2.0f, 0.0f);

    m_Sprite->SetWorldScale(100.0f, 100.0f, 1.0f);

    m_Mass = 250.0f;

    return true;
}

void CAttractor::Start()
{
    CGameObject::Start();
}

void CAttractor::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CAttractor::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CAttractor* CAttractor::Clone()
{
    return new CAttractor(*this);
}

void CAttractor::Attract(CGameObject* obj, float deltaTime)
{
    Vector2 force = Vector2(m_Sprite->GetWorldPos().x - obj->GetRootComponent()->GetWorldPos().x,
        m_Sprite->GetWorldPos().y - obj->GetRootComponent()->GetWorldPos().y);

    float dist = force.Length();

    dist = std::fmax(5.0f, std::fmin(dist, 100.0f));

    force.Normalize();

    float strength = (m_G * m_Mass * ((CMover*)obj)->GetMass()) / ((dist * dist));

    force *= strength;

    float rotation = m_Sprite->GetWorldPos().x - obj->GetRootComponent()->GetWorldPos().x;

    rotation = std::fmaxf(-0.01f, std::fminf(0.01f, rotation));

    ((CMover*)obj)->AddAngularForceX(-rotation);

    ((CMover*)obj)->ApplyForce(force * deltaTime);
}
