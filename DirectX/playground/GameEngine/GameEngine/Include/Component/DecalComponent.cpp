#include "DecalComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/Material/Material.h"
#include "../GameObject/GameObject.h"

CDecalComponent::CDecalComponent() :
	m_FadeInTime(0.f),
	m_FadeInTimeAcc(0.f),
	m_FadeOutTime(0.f),
	m_FadeOutTimeAcc(0.f),
	m_DurationTime(0.f),
	m_DurationTimeAcc(0.f),
	m_FadeLoop(false),
	m_FadeState(Decal_Fade_State::Duration)
{
	SetTypeID<CDecalComponent>();
	m_Render = true;
}

CDecalComponent::CDecalComponent(const CDecalComponent& com) :
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

#if _DEBUG
	m_DebugMesh = com.m_DebugMesh;
	m_DebugMaterial = com.m_DebugMaterial;
#endif // _DEBUG

	m_FadeLoop = com.m_FadeLoop;
	m_FadeInTime = com.m_FadeInTime;
	m_FadeInTimeAcc = com.m_FadeInTimeAcc;
	m_FadeOutTime = com.m_FadeOutTime;
	m_FadeOutTimeAcc = com.m_FadeOutTimeAcc;
	m_DurationTime = com.m_DurationTime;
	m_DurationTimeAcc = com.m_DurationTimeAcc;
	m_FadeState = com.m_FadeState;

	if (com.m_Material)
		m_Material = com.m_Material->Clone();
}

CDecalComponent::~CDecalComponent()
{
}

void CDecalComponent::Start()
{
	CSceneComponent::Start();

	if (m_FadeInTime > 0.0f)
	{
		m_FadeState = Decal_Fade_State::FadeIn;

		if (m_Material)
			m_Material->SetOpacity(0.0f);
	}
	else if (m_FadeOutTime > 0.0f)
	{
		m_FadeState = Decal_Fade_State::FadeOut;

		if (m_Material)
			m_Material->SetOpacity(1.0f);
	}
	else
		m_FadeState = Decal_Fade_State::Duration;
}

bool CDecalComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

#ifdef _DEBUG
	m_DebugMesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh("CubeLinePos");
	m_DebugMaterial = m_Scene->GetSceneResource()->FindMaterial("DebugDecal")->Clone();
#endif // _DEBUG

	m_Mesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh("CubePos");
	m_Material = m_Scene->GetSceneResource()->FindMaterial("DefaultDecal")->Clone();

	SetRelativeScale(3.0f, 3.0f, 3.0f);

	return true;
}

void CDecalComponent::Update(float deltaTime)
{
CSceneComponent:Update(deltaTime);

	switch (m_FadeState)
	{
	case Decal_Fade_State::FadeIn:
		m_FadeInTimeAcc += deltaTime;

		m_Material->SetOpacity(m_FadeInTimeAcc / m_FadeInTime);

		if (m_FadeInTimeAcc >= m_FadeInTime)
		{
			m_FadeState = Decal_Fade_State::Duration;

			m_Material->SetOpacity(1.0f);
		}
		break;
	case Decal_Fade_State::FadeOut:
		m_FadeOutTimeAcc += deltaTime;

		m_Material->SetOpacity(1.0f - m_FadeOutTimeAcc / m_FadeOutTime);

		if (m_FadeOutTimeAcc >= m_FadeOutTime)
		{
			if (m_FadeLoop)
			{
				m_FadeInTimeAcc = 0.0f;
				m_FadeOutTimeAcc = 0.0f;
				m_DurationTimeAcc = 0.0f;

				if (m_FadeInTime > 0.0f)
				{
					m_FadeState = Decal_Fade_State::FadeIn;
					m_Material->SetOpacity(0.0f);
				}
				else
				{
					m_FadeState = Decal_Fade_State::Duration;
					m_Material->SetOpacity(1.0f);
				}
			}
			else
				m_Object->Destroy();
		}
		break;
	case Decal_Fade_State::Duration:
		if (m_DurationTime > 0.0f)
		{
			m_DurationTimeAcc += deltaTime;

			if (m_DurationTimeAcc >= m_DurationTime)
			{
				if (m_FadeOutTime > 0.0f)
					m_FadeState = Decal_Fade_State::FadeOut;
				else if (m_FadeLoop)
				{
					m_FadeInTimeAcc = 0.0f;
					m_FadeOutTimeAcc = 0.0f;
					m_DurationTimeAcc = 0.0f;

					if (m_FadeInTime > 0.0f)
					{
						m_FadeState = Decal_Fade_State::FadeIn;
						m_Material->SetOpacity(0.0f);
					}
				}
				else
					m_Object->Destroy();
			}
		}
		break;
	}
}

void CDecalComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CDecalComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CDecalComponent::Render()
{
	CSceneComponent::Render();

	m_Material->Render();

	m_Mesh->Render();

	m_Material->Reset();

#ifdef _DEBUG

	m_DebugMaterial->Render();

	m_DebugMesh->Render();

	m_DebugMaterial->Reset();
#endif // _DEBUG

}

void CDecalComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CDecalComponent* CDecalComponent::Clone()
{
	return new CDecalComponent(*this);
}

void CDecalComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CDecalComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
