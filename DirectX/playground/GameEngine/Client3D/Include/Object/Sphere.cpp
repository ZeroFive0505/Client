#include "Sphere.h"

CSphere::CSphere()
{
    SetTypeID<CSphere>();
}

CSphere::CSphere(const CSphere& obj)
{
    m_Collider = (CColliderSphere*)FindComponent("Collider");
}

CSphere::~CSphere()
{
}

bool CSphere::Init()
{
    m_Collider = CreateComponent<CColliderSphere>("Collider");

    m_Collider->SetInfo(Vector3(0.0f, 0.0f, 0.0f), 5.0f);

    m_Collider->SetCollisionProfile("Enemy");

    return true;
}

void CSphere::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CSphere::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CSphere* CSphere::Clone()
{
    return new CSphere(*this);
}
