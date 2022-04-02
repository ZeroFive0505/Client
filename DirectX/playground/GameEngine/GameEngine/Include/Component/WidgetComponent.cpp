#include "WidgetComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Device.h"
#include "../Engine.h"
#include "../Input.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"

CWidgetComponent::CWidgetComponent()
{
	SetTypeID<CWidgetComponent>();

	m_Space = WidgetComponent_Space::Screen;
	m_Render = true;
	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& com) :
	CSceneComponent(com)
{
	m_Space = com.m_Space;

	if (m_Space == WidgetComponent_Space::Screen)
	{
		SetInheritRotX(false);
		SetInheritRotY(false);
		SetInheritRotZ(false);
	}
	else
	{
		SetInheritRotX(true);
		SetInheritRotY(true);
		SetInheritRotZ(true);
	}

	m_WidgetWindow = com.m_WidgetWindow->Clone();
	m_WidgetWindow->m_OwnerComponent = this;
}

CWidgetComponent::~CWidgetComponent()
{
}

void CWidgetComponent::Start()
{
	CSceneComponent::Start();
}

bool CWidgetComponent::Init()
{
	SetLayerName("ScreenWidgetComponent");

	return true;
}

void CWidgetComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (m_WidgetWindow)
		m_WidgetWindow->Update(deltaTime);
}

void CWidgetComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);

	if (m_WidgetWindow)
	{
		// 월드 공간에서의 위치 정보로 WidgetComponent의 위치 정보가 갱신되어 있을 것이다.
		// 이를 화면 공간에서의 위치로 변환해 주어야 한다.
		// 월드 -> 뷰 -> 프로젝션 변환 후에 스크린 공간으로 변환이 된다.
		// 스크린 공간으로 변환이 될때에는 투영으로 변환된 x, y, z, w가 있을 경우
		// 모든 요소를 w로 나누어서 화면 공간으로 변환을 시도한다.
		CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

		Matrix matView = camera->GetViewMatrix();
		Matrix matProj = camera->GetProjMatrix();
		Matrix matVP = matView * matProj;

		// 현재 월드 공간에서의 위치를 가져온다.
		Vector3 worldPos = GetWorldPos();

		// 뷰 변환 후에 좌표를 구한다.
		Vector3 viewPos = worldPos.TransformCoord(matView);

		// 투영 후 좌표는 월도 공간에서의 좌표에서 뷰와 프로젝션 매트릭스를 통해서 이동시킨 좌표
		Vector3 projPos = viewPos.TransformCoord(matProj);

		if (m_Space == WidgetComponent_Space::World)
		{
			// 직접 x, y, z, w에서 w값을 구한다.
			float w = (viewPos.x * matProj._14) + 
				(viewPos.y * matProj._24) + 
				(viewPos.z * matProj._34) + matProj._44;

			projPos.x /= w;
			projPos.y /= w;
			projPos.z /= w;
		}

		sResolution rs = CDevice::GetInst()->GetResolution();

		Vector2 screenPos;
		// -1 ~ 1 사이의 좌표를 화면의 해상도 기준으로 바꿔준다.
		screenPos.x = projPos.x * (rs.width / 2.0f) + (rs.width / 2.0f);
		screenPos.y = projPos.y * (rs.height / 2.0f) + (rs.height / 2.0f);

		//if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
		//{

		//}
		//else
		//{

		//}

		m_WidgetWindow->SetPos(screenPos);

		m_WidgetWindow->PostUpdate(deltaTime);

		// m_WidgetWindow->CollisionMouse(CInput::GetInst()->GetMousePos());
	}
}

void CWidgetComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CWidgetComponent::Render()
{
	CRenderState* state = CRenderManager::GetInst()->FindRenderState("AlphaBlend");


	state->SetState();

	CSceneComponent::Render();

	if (m_WidgetWindow)
		m_WidgetWindow->Render();

	state->ResetState();
}

void CWidgetComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}

void CWidgetComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CWidgetComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
