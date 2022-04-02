#include "UIWidget.h"

CUIWidget::CUIWidget() :
    m_Zorder(0),
    m_Visibility(true)
{
}

CUIWidget::CUIWidget(const CUIWidget& widget)
{
    *this = widget;
}

CUIWidget::~CUIWidget()
{
}

bool CUIWidget::Init()
{
    return true;
}

void CUIWidget::Update(float deltaTime)
{
}

void CUIWidget::PostUpdate(float deltaTime)
{
}


void CUIWidget::Render(HDC hDC)
{
}

void CUIWidget::Render(const Vector2& pos, HDC hDC)
{
}

CUIWidget* CUIWidget::Clone()
{
    return nullptr;
}
