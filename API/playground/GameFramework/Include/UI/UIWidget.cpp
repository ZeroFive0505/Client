#include "UIWidget.h"

CUIWidget::CUIWidget() :
	m_ZOrder(0),
	m_Visibility(true),
	m_MouseHoverd(false)
{
}

CUIWidget::CUIWidget(const CUIWidget& widget)
{
	*this = widget;
	m_RefCount = 0;
}

CUIWidget::~CUIWidget()
{
}

bool CUIWidget::Init()
{
	return true;
}

void CUIWidget::Update(float DeltaTime)
{
}

void CUIWidget::PostUpdate(float DeltaTime)
{
}

void CUIWidget::Collision(float DeltaTime)
{
}

void CUIWidget::Render(HDC hDC)
{
}

void CUIWidget::Render(const Vector2& Pos, HDC hDC)
{
}

CUIWidget* CUIWidget::Clone()
{
	return nullptr;
}

bool CUIWidget::CollisionMouse(const Vector2& MousePos, float DeltaTime)
{
	if (m_Pos.x <= MousePos.x && MousePos.x <= m_Pos.x + m_Size.x &&
		m_Pos.y <= MousePos.y && MousePos.y <= m_Pos.y + m_Size.y)
	{
		// 마우스가 올려진적이 없으면.
		if (!m_MouseHoverd)
		{
			// 마우스 호버링 콜백함수를 호출한다.
			CollisionMouseHoveredCallback(DeltaTime);
		}

		return true;
	}

	return false;
}

void CUIWidget::CollisionMouseHoveredCallback(float DeltaTime)
{
	m_MouseHoverd = true;
	// MessageBox(nullptr, TEXT("Mouse Hovered."), TEXT("Mouse Collision Test"), MB_OK);
}

void CUIWidget::CollisionMouseReleaseCallback(float DeltaTime)
{
	m_MouseHoverd = false;
	// MessageBox(nullptr, TEXT("Mouse Released."), TEXT("Mouse Collision Test"), MB_OK);
}
