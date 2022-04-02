#include "DriverWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CDriverWidget::CDriverWidget() :
	m_Angle(0.0f),
	m_Selected(false),
	m_fElapsedTime(0.0f),
	m_Confirm(false)
{
}

CDriverWidget::CDriverWidget(const CDriverWidget& widget) :
	CUIWidget(widget),
	m_Animator(widget.m_Animator)
{
	m_BG = widget.m_BG;
	m_Angle = widget.m_Angle;
	m_Selected = widget.m_Selected;
}

CDriverWidget::~CDriverWidget()
{
}


bool CDriverWidget::Init()
{
	m_Scene->GetSceneResoruce()->LoadTexture("driver bg", TEXT("UI/DriverSelect/driver_bg.bmp"));

	m_BG = m_Scene->GetSceneResoruce()->FindTexture("driver bg");

	m_BG->SetColorKey(255, 0, 255);

	m_BG->SetScale(1.7f, 1.3f);

	return true;
}

void CDriverWidget::AnimatorInit(const std::string& name)
{
	m_Animator.Init(name, (CRaceScene*)m_Scene);
}

void CDriverWidget::Update(float deltaTime)
{
	if (m_Selected && !m_Confirm)
	{
		m_Angle = fmodf(m_Angle + 2.5f * deltaTime, (float)M_PI * 2.0f);
	}
	else if (m_Confirm)
	{
		m_Angle = (float)M_PI_2;
	}
	else
		m_Angle = 0.0f;

	m_fElapsedTime += deltaTime;
}

void CDriverWidget::PostUpdate(float deltaTime)
{
}

void CDriverWidget::Render(HDC hDC)
{
	m_BG->Render(hDC, m_Owner->GetPos() + m_Pos, Vector2(0.0f, 0.0f), Vector2(m_BG->GetWidth(), m_BG->GetHeight()));

	CTexture currentTexture = m_Animator.GetMinimapTexture(m_Angle, 1.0f);

	currentTexture.SetColorKey(255, 0, 255);

	currentTexture.SetScale(1.5f, 1.5f);

	float offset = 0.5f * cosf(m_fElapsedTime * 10.0f);

	if(!m_Selected)
		currentTexture.Render(hDC, m_Owner->GetPos() + Vector2(m_Pos.x, m_Pos.y + offset) + Vector2(16.0f, 16.0f), Vector2(0.0f, 0.0f), Vector2(currentTexture.GetWidth(), currentTexture.GetHeight()));
	else
		currentTexture.Render(hDC, m_Owner->GetPos() + Vector2(m_Pos.x, m_Pos.y) + Vector2(16.0f, 16.0f), Vector2(0.0f, 0.0f), Vector2(currentTexture.GetWidth(), currentTexture.GetHeight()));
}

void CDriverWidget::Render(const Vector2& pos, HDC hDC)
{

}

CDriverWidget* CDriverWidget::Clone()
{
	return new CDriverWidget(*this);
}
