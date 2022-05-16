#include "ColliderSphere.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "ColliderBox3D.h"

CColliderSphere::CColliderSphere()
{
    SetTypeID<CColliderSphere>();
    m_ComponentType = Component_Type::SceneComponent;
    m_Render = true;

    m_ColliderType = Collider_Type::Sphere;
}

CColliderSphere::CColliderSphere(const CColliderSphere& com) :
    CColliderComponent(com)
{
    m_Info = com.m_Info;
}

CColliderSphere::~CColliderSphere()
{
}

void CColliderSphere::Start()
{
    CColliderComponent::Start();
}

bool CColliderSphere::Init()
{
    if (!CColliderComponent::Init())
        return false;

    m_Info.radius = 50.0f;

    SetInheritRotX(true);
    SetInheritRotY(true);
    SetInheritRotZ(true);

    SetWorldScale(m_Info.radius * 2.0f, m_Info.radius * 2.0f, m_Info.radius * 2.0f);

    if (m_Scene)
        m_Mesh = m_Scene->GetSceneResource()->FindMesh("Sphere");
    else
        m_Mesh = CResourceManager::GetInst()->FindMesh("Sphere");

    return true;
}

void CColliderSphere::Update(float deltaTime)
{
    CColliderComponent::Update(deltaTime);
}

void CColliderSphere::PostUpdate(float deltaTime)
{
    CColliderComponent::PostUpdate(deltaTime);

    m_Info.center.x = GetWorldPos().x + m_Offset.x;
    m_Info.center.y = GetWorldPos().y + m_Offset.y;
    m_Info.center.z = GetWorldPos().z + m_Offset.z;

    m_Min.x = m_Info.center.x - m_Info.radius;
    m_Min.y = m_Info.center.y - m_Info.radius;
    m_Min.z = m_Info.center.z - m_Info.radius;

    m_Max.x = m_Info.center.x + m_Info.radius;
    m_Max.y = m_Info.center.y + m_Info.radius;
    m_Max.z = m_Info.center.z + m_Info.radius;

    m_Info.Min = m_Min;
    m_Info.Max = m_Max;
}

void CColliderSphere::PrevRender()
{
    CColliderComponent::PrevRender();
}

void CColliderSphere::Render()
{
    CColliderComponent::Render();

    CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

    Matrix matWorld, matView, matProj, matWVP;

    matView = camera->GetViewMatrix();
    matProj = camera->GetProjMatrix();

    Matrix matScale, matTrans;

    matScale.Scaling(m_Info.radius * 2.0f, m_Info.radius * 2.0f, m_Info.radius * 2.0f);
    matTrans.Translation(m_Info.center);

    matWorld = matScale * matTrans;

    matWVP = matWorld * matView * matProj;

    matWVP.Transpose();

    m_CBuffer->SetWVP(matWVP);

    if (m_PrevCollisionList.empty())
        m_CBuffer->SetColliderColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    else
        m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

    if(m_MouseCollision)
        m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

    m_CBuffer->UpdateCBuffer();

    m_Shader->SetShader();

    m_Mesh->Render();
}

void CColliderSphere::PostRender()
{
    CColliderComponent::PostRender();
}

CColliderSphere* CColliderSphere::Clone()
{
    return new CColliderSphere(*this);
}

void CColliderSphere::Save(FILE* pFile)
{
    CColliderComponent::Save(pFile);

    fwrite(&m_Info, sizeof(sSphereInfo), 1, pFile);
}

void CColliderSphere::Load(FILE* pFile)
{
    CColliderComponent::Load(pFile);

    fread(&m_Info, sizeof(sSphereInfo), 1, pFile);
}

bool CColliderSphere::Collision(CColliderComponent* dest)
{
    switch (dest->GetColliderType())
    {
    case Collider_Type::Box3D:
        return CCollision::CollisionBox3DToSphere((CColliderBox3D*)dest, this);
    case Collider_Type::Sphere:
        return CCollision::CollisionSphereToSphere(this, (CColliderSphere*)dest);
    }

    return false;
}

bool CColliderSphere::CollisionMouse(const Vector2& mousePos)
{
    return m_MouseCollision;
}

bool CColliderSphere::CollisionRay(const sRay& ray)
{
    return false;
}
