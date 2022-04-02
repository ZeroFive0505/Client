#include "RCGDoor.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Widget/RCGDoorHUD.h"
#include "../Widget/RCGMainWidget.h"

CRCGDoor::CRCGDoor() :
	m_WidgetComponent(nullptr),
	m_Transform(nullptr)
{
	SetTypeID<CRCGDoor>();
	m_Progress = 0.0f;
}

CRCGDoor::CRCGDoor(const CRCGDoor& obj) :
	CGameObject(obj)
{
	m_Transform = (CSceneComponent*)FindComponent("Door Collider");
	m_WidgetComponent = (CWidgetComponent*)FindComponent("Door HUD");
}

CRCGDoor::~CRCGDoor()
{
}

void CRCGDoor::Start()
{
}

bool CRCGDoor::Init()
{
	m_Transform = CreateComponent<CSceneComponent>("Door Collider");

	m_WidgetComponent = CreateComponent<CWidgetComponent>("Door HUD");

	m_DoorHUD = m_WidgetComponent->CreateWidgetWindow<CRCGDoorHUD>("HUD");

	m_Transform->AddChild(m_WidgetComponent);

	return true;
}

void CRCGDoor::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	if (!m_Widget->IsFadingOut() && CSceneManager::GetInst()->GetScene()->GetPlayerObject()->GetWorldPos().SqrDistance(GetWorldPos()) < 2500.0f)
	{
		if (GetAsyncKeyState('Z'))
		{
			m_Progress += deltaTime;
			if (m_Progress > 1.0f)
			{
				if (m_Widget)
					m_Widget->FadeOutStart();
				m_Progress = 1.0f;
			}

			m_DoorHUD->SetProgress(m_Progress);
		}
		else
		{
			m_Progress -= deltaTime;
			if (m_Progress <= 0.0f)
				m_Progress = 0.0f;

			m_DoorHUD->SetProgress(m_Progress);
		}
	}
	else if(!m_Widget->IsFadingOut())
	{
		m_Progress -= deltaTime;
		if (m_Progress <= 0.0f)
			m_Progress = 0.0f;

		m_DoorHUD->SetProgress(m_Progress);
	}
}

void CRCGDoor::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CRCGDoor* CRCGDoor::Clone()
{
	return new CRCGDoor(*this);
}
