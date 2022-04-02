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
		// ���� ���������� ��ġ ������ WidgetComponent�� ��ġ ������ ���ŵǾ� ���� ���̴�.
		// �̸� ȭ�� ���������� ��ġ�� ��ȯ�� �־�� �Ѵ�.
		// ���� -> �� -> �������� ��ȯ �Ŀ� ��ũ�� �������� ��ȯ�� �ȴ�.
		// ��ũ�� �������� ��ȯ�� �ɶ����� �������� ��ȯ�� x, y, z, w�� ���� ���
		// ��� ��Ҹ� w�� ����� ȭ�� �������� ��ȯ�� �õ��Ѵ�.
		CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

		Matrix matView = camera->GetViewMatrix();
		Matrix matProj = camera->GetProjMatrix();
		Matrix matVP = matView * matProj;

		// ���� ���� ���������� ��ġ�� �����´�.
		Vector3 worldPos = GetWorldPos();

		// �� ��ȯ �Ŀ� ��ǥ�� ���Ѵ�.
		Vector3 viewPos = worldPos.TransformCoord(matView);

		// ���� �� ��ǥ�� ���� ���������� ��ǥ���� ��� �������� ��Ʈ������ ���ؼ� �̵���Ų ��ǥ
		Vector3 projPos = viewPos.TransformCoord(matProj);

		if (m_Space == WidgetComponent_Space::World)
		{
			// ���� x, y, z, w���� w���� ���Ѵ�.
			float w = (viewPos.x * matProj._14) + 
				(viewPos.y * matProj._24) + 
				(viewPos.z * matProj._34) + matProj._44;

			projPos.x /= w;
			projPos.y /= w;
			projPos.z /= w;
		}

		sResolution rs = CDevice::GetInst()->GetResolution();

		Vector2 screenPos;
		// -1 ~ 1 ������ ��ǥ�� ȭ���� �ػ� �������� �ٲ��ش�.
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
