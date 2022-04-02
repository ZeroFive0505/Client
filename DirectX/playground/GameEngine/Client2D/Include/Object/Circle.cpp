#include "Circle.h"

CCircle::CCircle()
{
    SetTypeID<CCircle>();
}

CCircle::CCircle(const CCircle& obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Circle");
}

CCircle::~CCircle()
{
}

bool CCircle::Init()
{
    m_Mesh = CreateComponent<CStaticMeshComponent>("Circle");

    m_Mesh->SetMesh("Circle");
    m_Mesh->GetMaterial()->SetShader("LineShader");
    m_Mesh->SetBaseColor(0.0f, 1.0f, 0.0f, 1.0f);

    return true;
}

void CCircle::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CCircle::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CCircle* CCircle::Clone()
{
    return new CCircle(*this);
}
