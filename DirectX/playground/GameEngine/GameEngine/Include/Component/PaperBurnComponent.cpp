#include "PaperBurnComponent.h"
#include "../Resource/Material/Material.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CPaperBurnComponent::CPaperBurnComponent()
{
    SetTypeID<CPaperBurnComponent>();

    m_CBuffer = new CPaperBurnConstantBuffer;

    m_CBuffer->Init();

    m_FinishTime = 5.0f;

    m_StartPaperBurn = false;

    m_Filter = 0.0f;
}

CPaperBurnComponent::CPaperBurnComponent(const CPaperBurnComponent& com) :
    CObjectComponent(com)
{
    m_CBuffer = com.m_CBuffer->Clone();

    m_FinishTime = com.m_FinishTime;
    m_Material = com.m_Material;
    m_StartPaperBurn = false;
    m_Filter = 0.0f;
}

CPaperBurnComponent::~CPaperBurnComponent()
{
    SAFE_DELETE(m_CBuffer);

    if (m_Material)
        m_Material->DeleteRenderCallback(this);
}

void CPaperBurnComponent::StartPaperBurn()
{
    if (!m_Material || m_StartPaperBurn)
        return;

    m_Material->SetPaperBurn(true);
    m_StartPaperBurn = true;
}

void CPaperBurnComponent::SetMaterial(CMaterial* material)
{
    m_Material = material;

    m_Material->AddRenderCallback(this, &CPaperBurnComponent::SetShader);
}

void CPaperBurnComponent::SetInverse(bool enable)
{
    m_CBuffer->SetInverse(enable);
}

void CPaperBurnComponent::SetBurnTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetSceneResource()->LoadTexture(name, fileName, pathName))
            return;

        m_BurnTexture = m_Scene->GetSceneResource()->FindTexture(name);
    }
    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
            return;

        m_BurnTexture = CResourceManager::GetInst()->FindTexture(name);
    }
}

void CPaperBurnComponent::SetOutLineColor(const Vector4& color)
{
    m_CBuffer->SetOutColor(color);
}

void CPaperBurnComponent::SetOutLineColor(float r, float g, float b, float a)
{
    m_CBuffer->SetOutColor(r, g, b, a);
}

void CPaperBurnComponent::SetInLineColor(const Vector4& color)
{
    m_CBuffer->SetInColor(color);
}

void CPaperBurnComponent::SetInLineColor(float r, float g, float b, float a)
{
    m_CBuffer->SetInColor(r, g, b, a);
}

void CPaperBurnComponent::SetCenterLineColor(const Vector4& color)
{
    m_CBuffer->SetCenterColor(color);
}

void CPaperBurnComponent::SetCenterLineColor(float r, float g, float b, float a)
{
    m_CBuffer->SetCenterColor(r, g, b, a);
}

void CPaperBurnComponent::SetInFilter(float filter)
{
    m_CBuffer->SetInFilter(filter);
}

void CPaperBurnComponent::SetOutFilter(float filter)
{
    m_CBuffer->SetOutFilter(filter);
}

void CPaperBurnComponent::SetCenterFilter(float filter)
{
    m_CBuffer->SetCenterFilter(filter);
}

void CPaperBurnComponent::Start()
{
    CObjectComponent::Start();
}

bool CPaperBurnComponent::Init()
{
    SetBurnTexture("DefaultBurnTexture", TEXT("DefaultPaperBurn.png"));

    return true;
}

void CPaperBurnComponent::Update(float deltaTime)
{
    if (m_StartPaperBurn)
    {
        m_Filter += deltaTime / m_FinishTime;

        if (m_Filter >= 1.0f)
        {
            m_Filter = 1.0f;

            if (m_FinishCallback)
                m_FinishCallback();
        }

        m_CBuffer->SetFilter(m_Filter);
    }
}

void CPaperBurnComponent::PostUpdate(float deltaTime)
{
}

void CPaperBurnComponent::PrevRender()
{
}

void CPaperBurnComponent::Render()
{
}

void CPaperBurnComponent::PostRender()
{
}

CPaperBurnComponent* CPaperBurnComponent::Clone()
{
    return new CPaperBurnComponent(*this);
}

void CPaperBurnComponent::SetShader()
{
    m_CBuffer->UpdateCBuffer();

    m_BurnTexture->SetShader(101, (int)Buffer_Shader_Type::Pixel, 0);
}
