#include "WidgetComponent.h"
#include "../Object/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CWidgetComponent::CWidgetComponent() :
	m_Visibility(true),
	m_Owner(nullptr),
	m_Scene(nullptr),
	m_Widget(nullptr)
{
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& widget)
{
	m_Pos = widget.m_Pos;
	m_Visibility = widget.m_Visibility;
	m_Widget = widget.m_Widget->Clone();
}

CWidgetComponent::~CWidgetComponent()
{
}

bool CWidgetComponent::Init()
{
	return true;
}

void CWidgetComponent::Update(float DeltaTime)
{
	if (m_Widget)
	{
		m_Widget->Update(DeltaTime);
	}
}

void CWidgetComponent::PostUpdate(float DeltaTime)
{
	if (m_Widget)
	{
		m_Widget->PostUpdate(DeltaTime);
	}
}

void CWidgetComponent::Collision(float DeltaTime)
{
	if (m_Widget)
	{
		m_Widget->Collision(DeltaTime);
	}
}

void CWidgetComponent::Render(HDC hDC)
{
	// 위젯을 그릴떄는 소유자의 위치를 더 하고 카메라의 위치를 뺀다.
	// 카메라의 위치를 빼는 이유는 카메라가 오른쪽으로 움직일 경우
	// 물체는 왼쪽으로 움직이기 때문
	if (m_Widget)
	{
		Vector2 RenderPos = m_Pos + m_Owner->GetPos() - m_Scene->GetCamera()->GetPos();
		m_Widget->Render(RenderPos, hDC);
	}
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}
