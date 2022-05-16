#include "Box3D.h"

CBox3D::CBox3D()
{
    SetTypeID<CBox3D>();
}

CBox3D::CBox3D(const CBox3D& obj)
{
    m_Collider = (CColliderBox3D*)FindComponent("Collider");
}

CBox3D::~CBox3D()
{
}

bool CBox3D::Init()
{
    m_Collider = CreateComponent<CColliderBox3D>("Collider");

    m_Collider->SetExtent(5.0f, 5.0f, 5.0f);

    m_Collider->SetCollisionProfile("Enemy");

    return true;
}

void CBox3D::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CBox3D::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CBox3D* CBox3D::Clone()
{
    return new CBox3D(*this);
}
