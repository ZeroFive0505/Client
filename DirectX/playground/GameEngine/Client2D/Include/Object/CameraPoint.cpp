#include "CameraPoint.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CCameraPoint::CCameraPoint() :
    m_Camera(nullptr)
{
    SetTypeID<CCameraPoint>();
}

CCameraPoint::CCameraPoint(const CCameraPoint& obj) :
    CGameObject(obj)
{
    m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CCameraPoint::~CCameraPoint()
{

}

bool CCameraPoint::Init()
{
    m_Transform = CreateComponent<CSceneComponent>("Camera Transform");

    m_Camera = CreateComponent<CCameraComponent>("Camera");

    m_Transform->AddChild(m_Camera);

    m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

    SetPivot(0.5f, 0.5f, 0.0f);

    SetWorldPos(0.0f, 0.0f, 0.0f);

    m_Camera->OnViewPortCenter();

    m_Follow = true;

    return true;
}

void CCameraPoint::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);

    if (m_Target && m_Follow)
    {
        Vector3 pos = m_Target->GetWorldPos();

        if (m_Lerp)
        {
            m_LerpTime += deltaTime;

            if (m_LerpTime > 1.0f)
            {
                m_Lerp = 1.0f;
                m_Lerp = false;
            }

            float x = GetWorldPos().x;
            float y = GetWorldPos().y;

            float nx = x + m_LerpTime * (pos.x - x);
            float ny = y + m_LerpTime * (pos.y - y);

            pos.x = nx;
            pos.y = ny;

            SetWorldPos(pos);
        }
        else
            SetWorldPos(pos);
    }
}

void CCameraPoint::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CCameraPoint* CCameraPoint::Clone()
{
    return new CCameraPoint(*this);
}
