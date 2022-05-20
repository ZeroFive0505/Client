#include "Transform.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Scene/Scene.h"
#include "CameraComponent.h"
#include "../Resource/Animation/SkeletonSocket.h"

CTransform::CTransform() :
    m_Parent(nullptr),
    m_Scene(nullptr),
    m_Object(nullptr),
    m_Owner(nullptr),
    m_Socket(nullptr),
    m_InheritScale(false),
    m_InheritRotX(false),
    m_InheritRotY(false),
    m_InheritRotZ(false),
    m_InheritParentRotationPosX(false),
    m_InheritParentRotationPosY(false),
    m_InheritParentRotationPosZ(false),
    m_UpdateScale(true),
    m_UpdateRot(true),
    m_UpdatePos(true),
    m_WorldScale(1.0f, 1.0f, 1.0f),
    m_RelativeScale(1.0f, 1.0f, 1.0f),
    m_State(Transform_State::None)
{
    // ���� �ʱ�ȭ�Ѵ�.
    for (int i = 0; i < AXIS_MAX; i++)
    {
        m_RelativeAxis[i] = Vector3::Axis[i];
        m_WorldAxis[i] = Vector3::Axis[i];
    }
}

CTransform::CTransform(const CTransform& transform)
{
    *this = transform;

    // Ʈ������ ������Ʈ�� ����ɶ��� ������۴� ���� �����ؾ���
    m_CBuffer = transform.m_CBuffer->Clone();
}

CTransform::~CTransform()
{
    SAFE_DELETE(m_CBuffer);
}

void CTransform::InheritScale(bool currnet)
{
    // ���� �θ� �����ϰ� �������� ��ӹ޴´ٸ�
    if (m_Parent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

    // �����Ϻ�ȭ�� �������� ������ ��ȯ
    m_UpdateScale = true;
    
    // �ڽ��� ���� ��� ��� ����
    size_t size = m_vecChild.size();

    for (size_t i = 0; i < size; i++)
    {
        // ���⼭ �ڱ��ڽ��� �������� �ٲٴ°� �ƴϴ�
        // ������ �����Ѵ�.
        m_vecChild[i]->InheritScale(false);
    }
}

void CTransform::InheritRotation(bool current)
{
    // ���� �θ� ���� ��쿡��
    if (m_Parent)
    {
        // �θ��� ������ŭ ���ؼ� ���ο� ���忡���� ������ ���������.

        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;

        // ���� �� �ϳ��� �θ𿡰Լ� ����� �ް�
        // ���� ���� �ڱ��ڽ��� ȸ���̾ƴ϶��(�ڽ��̶�� ��)
        // �ڽ��� ��쵵 �θ��� ��ġ�� ������ ȸ���ϸ鼭 ���ο� ��ġ�� �̵��ϰ� �ȴ�.
        if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !current)
            InheritParentRotationPos(false);
    }

    // x, y, z, ��׸� ������ �������� ��ȯ
    Vector3 convertRot = m_RelativeRot.ConvertAngle();

    // Roll Pitch Yaw�� ���ʹϿ��� ������. (�������� �����ϱ� ���ؼ�)
    // Roll : z, Pitch : x, Yaw = y 
    XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

    Matrix matRot;
    matRot.RotationQuaternion(qut);

    for (int i = 0; i < AXIS_MAX; i++)
    {
        // ��ġ�� ����� ���� ȸ���� �ǽ��Ѵ�.
        // �ڱ� �ڽ��� ���̵Ǽ� ȸ���� �ϱ⿡ W�� ���� 0���� ȸ��
        // �ϴ� ���� �⺻�� �Ǵ� ����� �����´�. �� ���� ȸ�� ���� ������� ȸ������ŭ ȸ���ϸ�
        // �װ��� ���ο� ������� ȸ�����̵ȴ�.
        m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_RelativeAxis[i].Normalize();
    }

    convertRot = m_WorldRot.ConvertAngle();
    qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
    matRot.RotationQuaternion(qut);

    for (int i = 0; i < AXIS_MAX; i++)
    {
        // ��ġ�� ����� ���� ȸ���� �ǽ��Ѵ�.
        // �ڱ� �ڽ��� ���̵Ǽ� ȸ���� �ϱ⿡ W�� ���� 0���� ȸ��
         // �ϴ� ���� �⺻�� �Ǵ� ����� �����´�. �� ���� ȸ�� ���� ���忡���� ȸ������ŭ ȸ���ϸ�
        // �װ��� ���ο� ���忡�� ȸ�����̵ȴ�.
        m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_WorldAxis[i].Normalize();
    }

    m_UpdateRot = true;

    size_t size = m_vecChild.size();

    // �ڽ��� ȸ���� �����Ѵ�.
    for (size_t i = 0; i < size; i++)
    {
        // �ڽ��� ȸ���� ��ü�� �ƴϴ�
        m_vecChild[i]->InheritRotation(false);
    }
}

// �θ��� ȸ���� ��ӹް� ������� ������ ����������
void CTransform::InheritParentRotationPos(bool current)
{
    // ���� �θ� ���� ���
    if (m_Parent)
    {
        Vector3 parentRot;
        
        // ȸ����ġ�� �����´�.
        if (m_InheritRotX)
            parentRot.x = m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            parentRot.y = m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            parentRot.z = m_Parent->GetWorldRot().z;


        // ���� ȸ���� ��ӹ������� �θ��� ȸ���� ���ؼ� �ڽ��� ���ο� ��ġ�� ��������.
        if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
        {
            // ���� �θ��� ȸ����ġ�� ��ȯ�Ѵ�.
            Vector3 convertRot = parentRot.ConvertAngle();

            // ���ʹϿ����� ��ȯ
            XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

            // ���ʹϿ����� ȸ�� ����� �����.
            Matrix matRot;
            matRot.RotationQuaternion(qut);

            // �θ��� ��ǥ�� �����´�.
            Vector3 parentPos = m_Parent->GetWorldPos();

            // 41, 42, 43�� ��� ��Ŀ��� ��ġ�� ��Ÿ���� ������
            // ���⿡ �θ��� ��ǥ�� �����Ѵ�.
            memcpy(&matRot._41, &parentPos, sizeof(Vector3));

            // matRot.Inverse();
            // m_WorldPos = m_RelativePos * matRot
            // m_RealtivePos = m_WorldPos / matRot (���⼭ ����� �����Ⱑ �Ұ����ϱ⿡ ������� ������ ������ش�.)
            // m_RelativePos = m_WorldPos.TransformCoord(matRot);

            // ���忡���� ���ο� ��ġ�� �θ��� ��ġ�� ���������� ȸ�� ������ ��ǥ��.
            m_WorldPos = m_RelativePos.TransformCoord(matRot);
        }
        // ���� ȸ���� ��ӹ��� �ʴ´ٸ� �����ϰ� ��ġ�� �����Ѵ�.
        else
            m_WorldPos = m_RelativePos + m_Parent->GetWorldPos();
    }

    m_UpdatePos = true;

    size_t size = m_vecChild.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->InheritParentRotationPos(false);
    }
}

void CTransform::InheritWorldScale(bool currnet)
{
    // InheritScale�� ���� ����

    // m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
    // ������ �������� ����� �������� �κ��� �������� ������ �����
    // ����� �������� ��� �θ��� �������� �����ؼ� �����ش�.
    if (m_Parent && m_InheritScale)
        m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();

    m_UpdateScale = true;

    size_t size = m_vecChild.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->InheritWorldScale(false);
    }
}

void CTransform::InheritWorldRotation(bool current)
{
    // InheritRotation�� ���� �����ϴ�.
    if (m_Parent)
    {
        // m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
        // ȸ���� ��� ���� ȸ���� ����� ȸ����ġ�� �θ��� ȸ����ġ�� ���ߴ�.
        // �ݴ�� ����� ȸ���� ���ش�.
        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;

        if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !current)
            InheritParentRotationPos(false);
    }

    Vector3 convertRot = m_RelativeRot.ConvertAngle();

    XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

    Matrix matRot;

    matRot.RotationQuaternion(qut);

    for (int i = 0; i < AXIS_MAX; i++)
    {
        m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_RelativeAxis[i].Normalize();
    }

    convertRot = m_WorldRot.ConvertAngle();
    qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
    matRot.RotationQuaternion(qut);

    for (int i = 0; i < AXIS_MAX; i++)
    {
        m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_WorldAxis[i].Normalize();
    }

    m_UpdateRot = true;

    size_t size = m_vecChild.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->InheritWorldRotation(false);
    }
}

// �θ��� ȸ���� ��ӹް� ���忡�� �����϶�
void CTransform::InheritParentRotationWolrdPos(bool currnet)
{
    // ���� �θ� �����Ѵٸ�
    if (m_Parent)
    {
        Vector3 parentRot;

        if (m_InheritRotX)
            parentRot.x = m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            parentRot.y = m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            parentRot.z = m_Parent->GetWorldRot().z;

        // �θ��� ȸ���� ��ӹ޴´ٸ�
        if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
        {
            // �θ��� ȸ���� �����´�.
            Vector3 convertRot = parentRot.ConvertAngle();

            XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

            Matrix matRot;

            // ȸ�� ����� �����.
            matRot.RotationQuaternion(qut);

            Vector3 parentPos = m_Parent->GetWorldPos();

            // �θ��� ��ġ�� ������� ȸ������ �����
            memcpy(&matRot._41, &parentPos, sizeof(Vector3));

            
            matRot.Inverse();

            // ���� ��ġ�� �����ġ * ȸ������� ��.
            // ���⼭ �����ġ�� ���ؾ��ϴ� ������ġ�� ȸ������� ����� ���� �����ش�.
            m_RelativePos = m_WorldPos.TransformCoord(matRot);
        }
        // ���� ȸ���� ��ӹ����ʴ´ٸ� �����ϰ� ��ġ�� �����Ѵ�.
        else
        {
            // m_WorldPos = m_RelativePos + m_Parent->GetWorldPos();
            m_RelativePos = m_WorldPos - m_Parent->GetWorldPos();
        }
    }

    m_UpdatePos = true;

    size_t size = m_vecChild.size();

    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->InheritParentRotationPos(false);
    }
}

void CTransform::SetRelativeScale(const Vector3& scale)
{
    m_RelativeScale = scale;
    
    m_WorldScale = scale;

    // ���� �� ��ü�� �������� �������ȴ�.
    // ���� ���� ����
    InheritScale(true);
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
    SetRelativeScale(Vector3(x, y, z));
}

void CTransform::MulRelativeScale(float x, float y, float z)
{
    Vector3 scale = Vector3(m_RelativeScale.x * x, m_RelativeScale.y * y, m_RelativeScale.z * z);

    SetRelativeScale(scale);
}

void CTransform::SetRelativeRotation(const Vector3& rot)
{
    m_RelativeRot = rot;

    m_WorldRot = rot;

    // ���� �� ��ü�� ȸ���� �������ȴ�.
    // ���� ���� ����
    InheritRotation(true);
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
    SetRelativeRotation(Vector3(x, y, z));
}

void CTransform::SetRelativeRotationX(float x)
{
    // x�ุ ���Ӱ� ����
    Vector3 rot(x, m_RelativeRot.y, m_RelativeRot.z);

    SetRelativeRotation(rot);
}

void CTransform::SetRelativeRotationY(float y)
{
    // y�ุ ���Ӱ� ����
    Vector3 rot(m_RelativeRot.x, y, m_RelativeRot.z);

    SetRelativeRotation(rot);
}

void CTransform::SetRelativeRotationZ(float z)
{
    // z�ุ ���Ӱ� ����
    Vector3 rot(m_RelativeRot.x, m_RelativeRot.y, z);

    SetRelativeRotation(rot);
}

void CTransform::SetRelativePos(const Vector3& pos)
{
    // �������� ��ü���Ǵ� ��ü�� ������ ���.

    // �����ǥ�� ���� ��ǥ�� ����
    m_RelativePos = pos;

    // ������ǥ�� ��������
    m_WorldPos = pos;

    InheritParentRotationPos(true);
}

void CTransform::SetRelativePos(float x, float y, float z)
{
    Vector3 pos(x, y, z);

    SetRelativePos(pos);
}

void CTransform::AddRelativeScale(const Vector3& scale)
{
    // �ڱ� �ڽ��� �������� �����Ѵ�.
    // ������ ��ġ��ŭ �����ش�.
    m_RelativeScale += scale;

    // ���忡�� �����ϵ� �״�� ���󰣴�.
    m_WorldScale = m_RelativeScale;

    InheritScale(true);
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
    Vector3 scale(x, y, z);

    AddRelativeScale(scale);
}

void CTransform::AddRelativeRotation(const Vector3& rot)
{
    // ȸ�� ��ġ�� �����ش�.
    m_RelativeRot += rot;

    m_WorldRot = m_RelativeRot;

    InheritRotation(true);
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
    Vector3 rot(x, y, z);

    AddRelativeRotation(rot);
}

void CTransform::AddRelativeRotationX(float x)
{
    Vector3 rot(x, 0.0f, 0.0f);

    AddRelativeRotation(rot);
}

void CTransform::AddRelativeRotationY(float y)
{
    Vector3 rot(0.0f, y, 0.0f);

    AddRelativeRotation(rot);
}

void CTransform::AddRelativeRotationZ(float z)
{
    Vector3 rot(0.0f, 0.0f, z);

    AddRelativeRotation(rot);
}

void CTransform::AddRelativePos(const Vector3& pos)
{
    m_RelativePos += pos;

    m_WorldPos = m_RelativePos;

    InheritParentRotationPos(true);
}

void CTransform::AddRelativePos(float x, float y, float z)
{
    Vector3 pos(x, y, z);

    AddRelativePos(pos);
}


void CTransform::SetWorldScale(const Vector3& scale)
{
    m_WorldScale = scale;

    m_RelativeScale = m_WorldScale;

    InheritWorldScale(true);
}

void CTransform::SetWorldScale(float x, float y, float z)
{
    Vector3 scale(x, y, z);

    SetWorldScale(scale);
}

void CTransform::MulWorldScale(float x, float y, float z)
{
    Vector3 scale = Vector3(m_WorldScale.x * x, m_WorldScale.y * y, m_WorldScale.z * z);

    SetWorldScale(scale);
}

void CTransform::SetWorldRotation(const Vector3& rot)
{
    m_WorldRot = rot;

    m_RelativeRot = m_WorldRot;

    InheritWorldRotation(true);
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
    Vector3 rot(x, y, z);

    SetWorldRotation(rot);
}

void CTransform::SetWorldRotationX(float x)
{
    Vector3 rot(x, m_WorldRot.y, m_WorldRot.z);

    SetWorldRotation(rot);
}

void CTransform::SetWorldRotationY(float y)
{
    Vector3 rot(m_WorldRot.x, y, m_WorldRot.z);

    SetWorldRotation(rot);
}

void CTransform::SetWorldRotationZ(float z)
{
    Vector3 rot(m_WorldRot.x, m_WorldRot.y, z);

    SetWorldRotation(rot);
}


void CTransform::SetWorldPos(const Vector3& pos)
{
    m_WorldPos = pos;
    m_RelativePos = pos;

    InheritParentRotationWolrdPos(true);
}

void CTransform::SetWorldPos(float x, float y, float z)
{
    Vector3 pos(x, y, z);

    SetWorldPos(pos);
}

void CTransform::AddWorldScale(const Vector3& scale)
{
    m_WorldScale += scale;

    m_RelativeScale = m_WorldScale;

    InheritWorldScale(true);
}

void CTransform::AddWorldScale(float x, float y, float z)
{
    Vector3 scale(x, y, z);

    AddWorldScale(scale);
}

void CTransform::AddWorldRotation(const Vector3& rot)
{
    m_WorldRot += rot;

    m_RelativeRot = m_WorldRot;

    InheritWorldRotation(true);
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
    Vector3 rot(x, y, z);

    AddWorldRotation(rot);
}

void CTransform::AddWorldRotationX(float x)
{
    Vector3 rot(x, 0.0f, 0.0f);

    AddWorldRotation(rot);
}

void CTransform::AddWorldRotationY(float y)
{
    Vector3 rot(0.0f, y, 0.0f);

    AddWorldRotation(rot);
}

void CTransform::AddWorldRotationZ(float z)
{
    Vector3 rot(0.0f, 0.0f, z);

    AddWorldRotation(rot);
}


void CTransform::AddWorldPos(const Vector3& pos)
{
    m_WorldPos += pos;
    m_RelativePos = m_WorldPos;

    InheritParentRotationWolrdPos(true);
}

void CTransform::AddWorldPos(float x, float y, float z)
{
    Vector3 pos(x, y, z);

    AddWorldPos(pos);
}


void CTransform::Start()
{
    // ó������ ��� �����ϰ� ȸ�� ��ġ�� ��ӹ޴´�.
    InheritScale(true);
    InheritRotation(true);
    InheritParentRotationPos(true);
}

void CTransform::Init()
{
    m_CBuffer = new CTransformConstantBuffer;

    m_CBuffer->Init();
}

void CTransform::Update(float deltaTime)
{
}

void CTransform::PostUpdate(float deltaTime)
{
    if (m_State == Transform_State::Ground)
    {
        float height = m_Scene->GetNavigation3DManager()->GetLandScapeHeight(m_WorldPos);

        if (height != m_WorldPos.y)
        {
            m_WorldPos.y = height;
            m_RelativePos = m_WorldPos;

            InheritParentRotationWolrdPos(true);
        }
    }



    // ������ ���� ��� �θ�� ������ �����ش�.
    if (m_Socket)
    {   
        // ������ ���� ���� ����� ���� �������ش�.

        if (m_UpdateScale)
            m_matScale.Scaling(m_RelativeScale);

        if (m_UpdateRot)
            m_matRot.Rotation(m_RelativeRot);

        if (m_UpdatePos)
            m_matPos.Translation(m_RelativePos);

        if (m_UpdateScale || m_UpdateRot || m_UpdatePos)
            m_matWorld = m_matScale * m_matRot * m_matPos;

        m_matWorld *= m_Socket->GetSocketMatrix();
    }
    else
    {
        Vector3 worldPos = m_WorldPos;

        if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
        {
            worldPos.z = worldPos.y / 30000.0f * 1000.0f; // ī�޶��� �þ߰Ÿ��� ���ؼ� 0 ~ 1000������ ��ȯ
            worldPos.z = std::fmaxf(0.001f, worldPos.z);
        }

        // ���� �������� ��ȭ�� ���� ���
        // m_WorldScale�� ������ �����ϸ��Ѵ�.
        // ���⼭ m_WorldScale�� �̹� �θ��� �����Ͽ� ��ȭ�� ������ ������̹� �Ǿ��� ���̴�.
        if (m_UpdateScale)
            m_matScale.Scaling(m_WorldScale);
        // ���� ������ ��ȭ�� ������ ���
        // m_WolrdRot���� ������ ��ȭ��Ų��.
        if (m_UpdateRot)
            m_matRot.Rotation(m_WorldRot);

        if (m_UpdatePos)
            m_matPos.Translation(worldPos);

        // ���� �ϳ��� ������Ʈ�� ��ٸ�
        // ���ο� ���� ��Ʈ������ ������.
        // ���� ��� = (ũ��(S) * ȸ��(R) * �̵�(T)) * ���� * �θ��� ��ġ
        if (m_UpdateScale || m_UpdateRot || m_UpdatePos)
            m_matWorld = m_matScale * m_matRot * m_matPos;

    }
}

void CTransform::SetTransform()
{
    // ������۸� ���ؼ� ������۸� �����Ѵ�.
    m_CBuffer->SetWorldMatrix(m_matWorld);

    CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();
    

    // ���� ī�޶��� ��, �������� ����� �����Ѵ�.
    m_CBuffer->SetViewMatrix(camera->GetViewMatrix());
    m_CBuffer->SetProjMatrix(camera->GetProjMatrix());

    m_CBuffer->SetPivot(m_Pivot);
    m_CBuffer->SetMeshSize(m_MeshSize);

    m_CBuffer->UpdateCBuffer();
}

void CTransform::ComputeWorld()
{
    m_matWorld = m_matScale * m_matRot * m_matPos;

    // ���� ������ �������
    if (m_Socket)
        m_matWorld *= m_Socket->GetSocketMatrix();
}

CTransform* CTransform::Clone()
{
    return new CTransform(*this);
}

void CTransform::Save(FILE* pFile)
{
    fwrite(&m_InheritScale, sizeof(bool), 1, pFile);
    fwrite(&m_InheritRotX, sizeof(bool), 1, pFile);
    fwrite(&m_InheritRotY, sizeof(bool), 1, pFile);
    fwrite(&m_InheritRotZ, sizeof(bool), 1, pFile);

    fwrite(&m_InheritParentRotationPosX, sizeof(bool), 1, pFile);
    fwrite(&m_InheritParentRotationPosY, sizeof(bool), 1, pFile);
    fwrite(&m_InheritParentRotationPosZ, sizeof(bool), 1, pFile);

    fwrite(&m_RelativeScale, sizeof(Vector3), 1, pFile);
    fwrite(&m_RelativeRot, sizeof(Vector3), 1, pFile);
    fwrite(&m_RelativePos, sizeof(Vector3), 1, pFile);
    fwrite(&m_RelativeAxis, sizeof(Vector3), AXIS_MAX, pFile);

    fwrite(&m_WorldScale, sizeof(Vector3), 1, pFile);
    fwrite(&m_WorldRot, sizeof(Vector3), 1, pFile);
    fwrite(&m_WorldPos, sizeof(Vector3), 1, pFile);
    fwrite(&m_WorldAxis, sizeof(Vector3), AXIS_MAX, pFile);
    fwrite(&m_Pivot, sizeof(Vector3), 1, pFile);
    fwrite(&m_MeshSize, sizeof(Vector3), 1, pFile);
}

void CTransform::Load(FILE* pFile)
{
    fread(&m_InheritScale, sizeof(bool), 1, pFile);
    fread(&m_InheritRotX, sizeof(bool), 1, pFile);
    fread(&m_InheritRotY, sizeof(bool), 1, pFile);
    fread(&m_InheritRotZ, sizeof(bool), 1, pFile);
  
    fread(&m_InheritParentRotationPosX, sizeof(bool), 1, pFile);
    fread(&m_InheritParentRotationPosY, sizeof(bool), 1, pFile);
    fread(&m_InheritParentRotationPosZ, sizeof(bool), 1, pFile);
   
    fread(&m_RelativeScale, sizeof(Vector3), 1, pFile);
    fread(&m_RelativeRot, sizeof(Vector3), 1, pFile);
    fread(&m_RelativePos, sizeof(Vector3), 1, pFile);
    fread(&m_RelativeAxis, sizeof(Vector3), AXIS_MAX, pFile);

    fread(&m_WorldScale, sizeof(Vector3), 1, pFile);
    fread(&m_WorldRot, sizeof(Vector3), 1, pFile);
    fread(&m_WorldPos, sizeof(Vector3), 1, pFile);
    fread(&m_WorldAxis, sizeof(Vector3), AXIS_MAX, pFile);
    fread(&m_Pivot, sizeof(Vector3), 1, pFile);
    fread(&m_MeshSize, sizeof(Vector3), 1, pFile);
}
