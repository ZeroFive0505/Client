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
    // 축을 초기화한다.
    for (int i = 0; i < AXIS_MAX; i++)
    {
        m_RelativeAxis[i] = Vector3::Axis[i];
        m_WorldAxis[i] = Vector3::Axis[i];
    }
}

CTransform::CTransform(const CTransform& transform)
{
    *this = transform;

    // 트랜스폼 컴포넌트가 복사될때는 상수버퍼는 따로 복사해야함
    m_CBuffer = transform.m_CBuffer->Clone();
}

CTransform::~CTransform()
{
    SAFE_DELETE(m_CBuffer);
}

void CTransform::InheritScale(bool currnet)
{
    // 만약 부모가 존재하고 스케일을 상속받는다면
    if (m_Parent && m_InheritScale)
        m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

    // 스케일변화가 생겼으니 참으로 변환
    m_UpdateScale = true;
    
    // 자식이 있을 경우 모두 갱신
    size_t size = m_vecChild.size();

    for (size_t i = 0; i < size; i++)
    {
        // 여기서 자기자신의 스케일을 바꾸는게 아니니
        // 거짓을 전달한다.
        m_vecChild[i]->InheritScale(false);
    }
}

void CTransform::InheritRotation(bool current)
{
    // 만약 부모가 있을 경우에는
    if (m_Parent)
    {
        // 부모의 각도만큼 더해서 새로운 월드에서의 각도가 만들어진다.

        if (m_InheritRotX)
            m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;

        // 만약 축 하나라도 부모에게서 상속을 받고
        // 만약 현재 자기자신의 회전이아니라면(자식이라는 뜻)
        // 자식의 경우도 부모의 위치를 축으로 회전하면서 새로운 위치로 이동하게 된다.
        if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !current)
            InheritParentRotationPos(false);
    }

    // x, y, z, 디그리 값에서 라디안으로 변환
    Vector3 convertRot = m_RelativeRot.ConvertAngle();

    // Roll Pitch Yaw로 쿼터니온을 만들어낸다. (짐벌락을 방지하기 위해서)
    // Roll : z, Pitch : x, Yaw = y 
    XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

    Matrix matRot;
    matRot.RotationQuaternion(qut);

    for (int i = 0; i < AXIS_MAX; i++)
    {
        // 위치가 상관이 없는 회전을 실시한다.
        // 자기 자신이 축이되서 회전을 하기에 W의 값은 0으로 회전
        // 일단 가장 기본이 되는 축들을 가져온다. 그 축을 회전 값만 상대적인 회전값만큼 회전하면
        // 그것이 새로운 상대적인 회전축이된다.
        m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_RelativeAxis[i].Normalize();
    }

    convertRot = m_WorldRot.ConvertAngle();
    qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
    matRot.RotationQuaternion(qut);

    for (int i = 0; i < AXIS_MAX; i++)
    {
        // 위치가 상관이 없는 회전을 실시한다.
        // 자기 자신이 축이되서 회전을 하기에 W의 값은 0으로 회전
         // 일단 가장 기본이 되는 축들을 가져온다. 그 축을 회전 값만 월드에서의 회전값만큼 회전하면
        // 그것이 새로운 월드에서 회전축이된다.
        m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_WorldAxis[i].Normalize();
    }

    m_UpdateRot = true;

    size_t size = m_vecChild.size();

    // 자식의 회전을 갱신한다.
    for (size_t i = 0; i < size; i++)
    {
        // 자식은 회전의 주체가 아니다
        m_vecChild[i]->InheritRotation(false);
    }
}

// 부모의 회전을 상속받고 상대적인 축으로 움직였을때
void CTransform::InheritParentRotationPos(bool current)
{
    // 만약 부모가 있을 경우
    if (m_Parent)
    {
        Vector3 parentRot;
        
        // 회전수치를 가져온다.
        if (m_InheritRotX)
            parentRot.x = m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            parentRot.y = m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            parentRot.z = m_Parent->GetWorldRot().z;


        // 만약 회전을 상속받을경우는 부모의 회전에 의해서 자식의 새로운 위치가 정해진다.
        if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
        {
            // 먼저 부모의 회전수치를 변환한다.
            Vector3 convertRot = parentRot.ConvertAngle();

            // 쿼터니온으로 변환
            XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

            // 쿼터니온으로 회전 행렬을 만든다.
            Matrix matRot;
            matRot.RotationQuaternion(qut);

            // 부모의 좌표를 가져온다.
            Vector3 parentPos = m_Parent->GetWorldPos();

            // 41, 42, 43의 경우 행렬에서 위치를 나타내는 변수다
            // 여기에 부모의 좌표를 복사한다.
            memcpy(&matRot._41, &parentPos, sizeof(Vector3));

            // matRot.Inverse();
            // m_WorldPos = m_RelativePos * matRot
            // m_RealtivePos = m_WorldPos / matRot (여기서 행렬은 나누기가 불가능하기에 역행렬을 위에서 만들어준다.)
            // m_RelativePos = m_WorldPos.TransformCoord(matRot);

            // 월드에서의 새로운 위치는 부모의 위치를 축으로했을 회전 이후의 좌표다.
            m_WorldPos = m_RelativePos.TransformCoord(matRot);
        }
        // 만약 회전을 상속받지 않는다면 간단하게 위치만 갱신한다.
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
    // InheritScale과 거의 유사

    // m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
    // 월드의 스케일은 상대적 스케일의 부보의 스케일을 곱해준 결과물
    // 상대적 스케일의 경우 부모의 스케일을 이항해서 나눠준다.
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
    // InheritRotation과 거의 유사하다.
    if (m_Parent)
    {
        // m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
        // 회전의 경우 월드 회전은 상대적 회전수치에 부모의 회전수치를 더했다.
        // 반대로 상대적 회전은 빼준다.
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

// 부모의 회전을 상속받고 월드에서 움직일때
void CTransform::InheritParentRotationWolrdPos(bool currnet)
{
    // 만약 부모가 존재한다면
    if (m_Parent)
    {
        Vector3 parentRot;

        if (m_InheritRotX)
            parentRot.x = m_Parent->GetWorldRot().x;

        if (m_InheritRotY)
            parentRot.y = m_Parent->GetWorldRot().y;

        if (m_InheritRotZ)
            parentRot.z = m_Parent->GetWorldRot().z;

        // 부모의 회전을 상속받는다면
        if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
        {
            // 부모의 회전을 가져온다.
            Vector3 convertRot = parentRot.ConvertAngle();

            XMVECTOR qut = XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);

            Matrix matRot;

            // 회전 행렬을 만든다.
            matRot.RotationQuaternion(qut);

            Vector3 parentPos = m_Parent->GetWorldPos();

            // 부모의 위치를 기반으로 회전축을 만든다
            memcpy(&matRot._41, &parentPos, sizeof(Vector3));

            
            matRot.Inverse();

            // 월드 위치는 상대위치 * 회전행렬의 값.
            // 여기서 상대위치를 구해야하니 월드위치에 회전행렬의 역행렬 값을 곱해준다.
            m_RelativePos = m_WorldPos.TransformCoord(matRot);
        }
        // 만약 회전을 상속받지않는다면 간단하게 위치를 갱신한다.
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

    // 현재 이 물체의 스케일이 재조정된다.
    // 따라서 참을 전달
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

    // 현재 이 물체의 회전이 재조정된다.
    // 따라서 참을 전달
    InheritRotation(true);
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
    SetRelativeRotation(Vector3(x, y, z));
}

void CTransform::SetRelativeRotationX(float x)
{
    // x축만 새롭게 갱신
    Vector3 rot(x, m_RelativeRot.y, m_RelativeRot.z);

    SetRelativeRotation(rot);
}

void CTransform::SetRelativeRotationY(float y)
{
    // y축만 새롭게 갱신
    Vector3 rot(m_RelativeRot.x, y, m_RelativeRot.z);

    SetRelativeRotation(rot);
}

void CTransform::SetRelativeRotationZ(float z)
{
    // z축만 새롭게 갱신
    Vector3 rot(m_RelativeRot.x, m_RelativeRot.y, z);

    SetRelativeRotation(rot);
}

void CTransform::SetRelativePos(const Vector3& pos)
{
    // 움직임의 주체가되는 물체가 움직인 경우.

    // 상대좌표도 들어온 좌표로 갱신
    m_RelativePos = pos;

    // 월드좌표도 마찬가지
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
    // 자기 자신의 스케일을 조정한다.
    // 스케일 수치만큼 더해준다.
    m_RelativeScale += scale;

    // 월드에서 스케일도 그대로 따라간다.
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
    // 회전 수치를 더해준다.
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
    // 처음에는 모든 스케일과 회전 위치를 상속받는다.
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



    // 소켓이 있을 경우 부모로 소켓을 곱해준다.
    if (m_Socket)
    {   
        // 소켓이 있을 경우는 상대적 값을 전달해준다.

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
            worldPos.z = worldPos.y / 30000.0f * 1000.0f; // 카메라의 시야거리를 곱해서 0 ~ 1000까지로 변환
            worldPos.z = std::fmaxf(0.001f, worldPos.z);
        }

        // 만약 스케일의 변화가 있을 경우
        // m_WorldScale의 값으로 스케일링한다.
        // 여기서 m_WorldScale은 이미 부모의 스케일에 변화가 있을때 계산이이미 되어진 값이다.
        if (m_UpdateScale)
            m_matScale.Scaling(m_WorldScale);
        // 만약 각도의 변화가 생겼을 경우
        // m_WolrdRot으로 각도를 변화시킨다.
        if (m_UpdateRot)
            m_matRot.Rotation(m_WorldRot);

        if (m_UpdatePos)
            m_matPos.Translation(worldPos);

        // 만약 하나라도 업데이트가 됬다면
        // 새로운 월드 매트릭스를 만들어낸다.
        // 월드 행렬 = (크기(S) * 회전(R) * 이동(T)) * 공전 * 부모의 위치
        if (m_UpdateScale || m_UpdateRot || m_UpdatePos)
            m_matWorld = m_matScale * m_matRot * m_matPos;

    }
}

void CTransform::SetTransform()
{
    // 상수버퍼를 통해서 월드버퍼를 전달한다.
    m_CBuffer->SetWorldMatrix(m_matWorld);

    CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();
    

    // 현재 카메라의 뷰, 프로젝션 행렬을 설정한다.
    m_CBuffer->SetViewMatrix(camera->GetViewMatrix());
    m_CBuffer->SetProjMatrix(camera->GetProjMatrix());

    m_CBuffer->SetPivot(m_Pivot);
    m_CBuffer->SetMeshSize(m_MeshSize);

    m_CBuffer->UpdateCBuffer();
}

void CTransform::ComputeWorld()
{
    m_matWorld = m_matScale * m_matRot * m_matPos;

    // 만약 소켓이 있을경우
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
