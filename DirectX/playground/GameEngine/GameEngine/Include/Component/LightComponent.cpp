#include "LightComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"

CLightComponent::CLightComponent()
{
    SetTypeID<CLightComponent>();
    m_Render = false;

    m_CBuffer = nullptr;
}

CLightComponent::CLightComponent(const CLightComponent& com) : 
    CSceneComponent(com)
{
    m_CBuffer = com.m_CBuffer->Clone();
}

CLightComponent::~CLightComponent()
{
    m_Scene->GetLightManager()->DeleteLight(this);
    SAFE_DELETE(m_CBuffer);
}

void CLightComponent::Start()
{
    CSceneComponent::Start();

    m_Scene->GetLightManager()->AddLight(this);
}

bool CLightComponent::Init()
{
    m_CBuffer = new CLightConstantBuffer;

    m_CBuffer->Init();

    return true;
}

void CLightComponent::Update(float deltaTime)
{
    CSceneComponent::Update(deltaTime);
}

void CLightComponent::PostUpdate(float deltaTime)
{
    CSceneComponent::PostUpdate(deltaTime);

    CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

    if (m_CBuffer->GetLightType() != Light_Type::Directional)
    {
        Vector3 pos = GetWorldPos();

        // 뷰 공간으로 전환한다.
        pos = pos.TransformCoord(camera->GetViewMatrix());

        m_CBuffer->SetPos(pos);

        if (m_CBuffer->GetLightType() == Light_Type::Point)
            SetRelativeScale(m_CBuffer->GetLightDistance(), m_CBuffer->GetLightDistance(), m_CBuffer->GetLightDistance());
    }

    if (m_CBuffer->GetLightType() != Light_Type::Point)
    {
        Vector3 dir = GetRelativeAxis(AXIS_Z);

        // 뷰 공간으로 전환한다.
        dir = dir.TransformNormal(camera->GetViewMatrix());
        dir.Normalize();

        m_CBuffer->SetDir(dir);
    }
}

void CLightComponent::PrevRender()
{
    CSceneComponent::PrevRender();
}

void CLightComponent::Render()
{
    CSceneComponent::Render();
}

void CLightComponent::PostRender()
{
    CSceneComponent::PostRender();
}

CLightComponent* CLightComponent::Clone()
{
    return new CLightComponent(*this);
}

void CLightComponent::Save(FILE* pFile)
{
    CSceneComponent::Save(pFile);
}

void CLightComponent::Load(FILE* pFile)
{
    CSceneComponent::Load(pFile);
}

void CLightComponent::SetShader()
{
    m_CBuffer->UpdateCBuffer();
}
