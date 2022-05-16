#include "ColliderBox3D.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "ColliderComponent.h"
#include "CameraComponent.h"
#include "ColliderSphere.h"
#include "../Scene/CameraManager.h"

CColliderBox3D::CColliderBox3D()
{
    SetTypeID<CColliderBox3D>();
    m_ComponentType = Component_Type::SceneComponent;
    m_Render = true;

    m_ColliderType = Collider_Type::Box3D;
}

CColliderBox3D::CColliderBox3D(const CColliderBox3D& com) :
    CColliderComponent(com)
{
    m_Info = com.m_Info;
}

CColliderBox3D::~CColliderBox3D()
{
}

void CColliderBox3D::Start()
{
    CColliderComponent::Start();
}

bool CColliderBox3D::Init()
{
    if (!CColliderComponent::Init())
        return false;

    m_Shader = CResourceManager::GetInst()->FindShader("Collider3D");

    // 초기에는 회전이 없다고 가정
    m_Info.axis[0] = Vector3(1.0f, 0.0f, 0.0f);
    m_Info.axis[1] = Vector3(0.0f, 1.0f, 0.0f);
    m_Info.axis[2] = Vector3(0.0f, 0.0f, 1.0f);

    m_Info.length = Vector3(50.0f, 50.0f, 50.0f);

    // X, Y, Z 축 회전을 상속
    SetInheritRotX(true);
    SetInheritRotY(true);
    SetInheritRotZ(true);

    SetWorldScale(m_Info.length.x * 2.0f, m_Info.length.y * 2.0f, m_Info.length.z * 2.0f);

    if (m_Scene)
        m_Mesh = m_Scene->GetSceneResource()->FindMesh("Box3D");
    else
        m_Mesh = CResourceManager::GetInst()->FindMesh("Box3D");

    return true;
}

void CColliderBox3D::Update(float deltaTime)
{
    CColliderComponent::Update(deltaTime);
}

void CColliderBox3D::PostUpdate(float deltaTime)
{
    CColliderComponent::PostUpdate(deltaTime);

    // 모든 업데이트가 끝난후에 회전을 계산

    // 충돌체의 중심점
    m_Info.center.x = GetWorldPos().x + m_Offset.x;
    m_Info.center.y = GetWorldPos().y + m_Offset.y;
    m_Info.center.z = GetWorldPos().z + m_Offset.z;

    // 회전 계산
    m_Info.axis[0].x = GetWorldAxis(AXIS_X).x;
    m_Info.axis[0].y = GetWorldAxis(AXIS_X).y;
    m_Info.axis[0].z = GetWorldAxis(AXIS_X).z;

    m_Info.axis[1].x = GetWorldAxis(AXIS_Y).x;
    m_Info.axis[1].y = GetWorldAxis(AXIS_Y).y;
    m_Info.axis[1].z = GetWorldAxis(AXIS_Y).z;

    m_Info.axis[2].x = GetWorldAxis(AXIS_Z).x;
    m_Info.axis[2].y = GetWorldAxis(AXIS_Z).y;
    m_Info.axis[2].z = GetWorldAxis(AXIS_Z).z;

    Vector3 pos[8] = {};

    // 육면체의 8개 꼭지점의 좌표를 구한다.
    
    // 앞면
    pos[0] = m_Info.center - m_Info.axis[0] * m_Info.length.x + m_Info.axis[1] * m_Info.length.y + m_Info.axis[2] * m_Info.length.z;
    pos[1] = m_Info.center + m_Info.axis[0] * m_Info.length.x + m_Info.axis[1] * m_Info.length.y + m_Info.axis[2] * m_Info.length.z;
    pos[2] = m_Info.center - m_Info.axis[0] * m_Info.length.x - m_Info.axis[1] * m_Info.length.y + m_Info.axis[2] * m_Info.length.z;
    pos[3] = m_Info.center + m_Info.axis[0] * m_Info.length.x - m_Info.axis[1] * m_Info.length.y + m_Info.axis[2] * m_Info.length.z;

    // 뒷면
    pos[4] = m_Info.center - m_Info.axis[0] * m_Info.length.x + m_Info.axis[1] * m_Info.length.y - m_Info.axis[2] * m_Info.length.z;
    pos[5] = m_Info.center + m_Info.axis[0] * m_Info.length.x + m_Info.axis[1] * m_Info.length.y - m_Info.axis[2] * m_Info.length.z;
    pos[6] = m_Info.center - m_Info.axis[0] * m_Info.length.x - m_Info.axis[1] * m_Info.length.y - m_Info.axis[2] * m_Info.length.z;
    pos[7] = m_Info.center + m_Info.axis[0] * m_Info.length.x - m_Info.axis[1] * m_Info.length.y - m_Info.axis[2] * m_Info.length.z;

    m_Min = pos[0];
    m_Max = pos[0];

    for (int i = 1; i < 8; i++)
    {
        if (m_Min.x > pos[i].x)
            m_Min.x = pos[i].x;

        if (m_Min.y > pos[i].y)
            m_Min.y = pos[i].y;
        
        if (m_Min.z > pos[i].z)
            m_Min.z = pos[i].z;

        if (m_Max.x < pos[i].x)
            m_Max.x = pos[i].x;

        if (m_Max.y < pos[i].y)
            m_Max.y = pos[i].y;

        if (m_Max.z < pos[i].z)
            m_Max.z = pos[i].z;
    }

    m_Info.Min = m_Min;
    m_Info.Max = m_Max;
}

void CColliderBox3D::PrevRender()
{
    CColliderComponent::PrevRender();
}

void CColliderBox3D::Render()
{
    CColliderComponent::Render();

    CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

    Matrix matWorld, matView, matProj, matWVP;

    matView = camera->GetViewMatrix();
    matProj = camera->GetProjMatrix();

    Matrix matScale, matRot, matTrans;

    matScale.Scaling(m_Info.length.x * 2.0f, m_Info.length.y * 2.0f, m_Info.length.z * 2.0f);
    matRot.Rotation(GetWorldRot());
    matTrans.Translation(m_Info.center);

    matWorld = matScale * matRot * matTrans;

    matWVP = matWorld * matView * matProj;

    matWVP.Transpose();

    m_CBuffer->SetWVP(matWVP);

    if (m_PrevCollisionList.empty())
        m_CBuffer->SetColliderColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    else
        m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

    m_CBuffer->UpdateCBuffer();

    m_Shader->SetShader();

    m_Mesh->Render();
}

void CColliderBox3D::PostRender()
{
    CColliderComponent::PostRender();
}

CColliderBox3D* CColliderBox3D::Clone()
{
    return new CColliderBox3D(*this);
}

void CColliderBox3D::Save(FILE* pFile)
{
    CColliderComponent::Save(pFile);

    fwrite(&m_Info, sizeof(sBox3DInfo), 1, pFile);
}

void CColliderBox3D::Load(FILE* pFile)
{
    CColliderComponent::Load(pFile);

    fwrite(&m_Info, sizeof(sBox3DInfo), 1, pFile);
}

bool CColliderBox3D::Collision(CColliderComponent* dest)
{
    switch (dest->GetColliderType())
    {
    case Collider_Type::Box3D:
        return CCollision::CollisionBox3DToBox3D(this, (CColliderBox3D*)dest);
    case Collider_Type::Sphere:
        return CCollision::CollisionBox3DToSphere(this, (CColliderSphere*)dest);
    }

    return false;
}

bool CColliderBox3D::CollisionMouse(const Vector2& mousePos)
{
    return false;
}
