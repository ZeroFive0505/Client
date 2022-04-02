#include "IMGUIWidget.h"

CIMGUIWidget::CIMGUIWidget() :
	m_Font(nullptr),
	m_Color{ 1.0f, 1.0f, 1.0f, 1.0f },
	m_Size(100.0f, 100.0f),
	m_Hide(false)
{
}

CIMGUIWidget::~CIMGUIWidget()
{
}

bool CIMGUIWidget::Init()
{
	return false;
}

void CIMGUIWidget::Render()
{
}
