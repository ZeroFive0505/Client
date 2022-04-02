#include "SliderWidget.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"

CSliderWidget::CSliderWidget() :
	m_Percent(1.0f),
	m_ProgressCBuffer(nullptr),
	m_StartMouseOn(false),
	m_Direction(ProgressBar_Direction::RightToLeft),
	m_SliderWidgetDown(false),
	m_SliderWidgetHold(false)
{
}

CSliderWidget::CSliderWidget(const CSliderWidget& widget) :
	CWidget(widget)
{
	m_Percent = widget.m_Percent;

	m_Direction = widget.m_Direction;

	m_ProgressCBuffer = new CProgressBarConstantBuffer;

	m_ProgressCBuffer->Init();

	m_ProgressCBuffer->SetPercent(m_Percent);
}

CSliderWidget::~CSliderWidget()
{
	SAFE_DELETE(m_ProgressCBuffer);
}

bool CSliderWidget::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return false;

		m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return false;

		m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

bool CSliderWidget::SetTexture(const std::string& name, const TCHAR* fullPath)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, fullPath))
			return false;
		
		m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
			return false;

		m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

void CSliderWidget::Start()
{
	CWidget::Start();
}

bool CSliderWidget::Init()
{
	if (!CWidget::Init())
		return false;

	SetZOrder(1);

	if (m_Owner->GetViewport())
		m_Shader = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindShader("ProgressBarShader");
	else
		m_Shader = CResourceManager::GetInst()->FindShader("ProgressBarShader");

	m_Background = new CImage;

	m_Background->m_Owner = m_Owner;

	m_Background->Init();

	m_Background->SetZOrder(0);

	m_Handle = new CImage;

	m_Handle->m_Owner = m_Owner;

	m_Handle->Init();

	m_Handle->SetZOrder(2);

	m_ProgressCBuffer = new CProgressBarConstantBuffer;

	m_ProgressCBuffer->Init();

	m_ProgressCBuffer->SetPercent(m_Percent);

	return true;
}

void CSliderWidget::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
	m_Background->Update(deltaTime);
	m_Handle->Update(deltaTime);

	if (m_MouseHovered && !m_StartMouseOn)
	{
		m_StartMouseOn = true;
		if (m_MouseOnCallback)
			m_MouseOnCallback();
	}
	else if (!m_MouseHovered && m_StartMouseOn)
	{
		m_StartMouseOn = false;
	}


	if (m_MouseHovered && CInput::GetInst()->GetMouseLButtonClick())
	{
		if (!m_SliderWidgetDown && !m_SliderWidgetHold)
		{
			m_SliderWidgetDown = true;
			m_SliderWidgetHold = true;
		}
		else if (m_SliderWidgetDown && m_SliderWidgetHold)
		{
			m_SliderWidgetDown = false;
			CInput::GetInst()->SetDragWidget(this);
		}
	}
	else if (!m_SliderWidgetHold || !CInput::GetInst()->GetMouseLButtonClick())
	{
		m_SliderWidgetDown = false;
		m_SliderWidgetHold = false;
		CInput::GetInst()->SetDragWidget(nullptr);
	}

	if (m_SliderWidgetHold &&
		CInput::GetInst()->GetDragWidget() != nullptr &&
		CInput::GetInst()->GetDragWidget() == this)
	{
		Vector2 mousePos = CInput::GetInst()->GetMousePos();
		
		switch (m_Direction)
		{
		case ProgressBar_Direction::RightToLeft:
		{
			Vector2 converMousePos = mousePos - m_Pos;
			float convertPos = converMousePos.x  / m_Size.x;
			m_Percent = convertPos;
			SetPercent(m_Percent);
			m_Handle->SetPos(m_Pos.x + (m_Percent * m_Size.x) - m_Handle->GetWindowSize().x / 2.0f, m_Handle->GetWindowPos().y);
		}
			break;
		case ProgressBar_Direction::LeftToRight:
		{
			Vector2 converMousePos = mousePos - m_Pos;
			float convertPos = converMousePos.x / m_Size.x;
			m_Percent = 1.0f - convertPos;
			SetPercent(m_Percent);
			m_Handle->SetPos(m_Pos.x + (m_Percent * m_Size.x) - m_Handle->GetWindowSize().x / 2.0f, m_Handle->GetWindowPos().y);
		}
			break;
		case ProgressBar_Direction::TopToBottom:
		{
			Vector2 converMousePos = mousePos - m_Pos;
			float convertPos = converMousePos.y / m_Size.y;
			m_Percent = 1.0f - convertPos;
			SetPercent(m_Percent);
			m_Handle->SetPos(m_Handle->GetWindowPos().x, m_Pos.y + (m_Percent * m_Size.y) - m_Handle->GetWindowSize().y / 2.0f);
		}
			break;
		case ProgressBar_Direction::BottomToTop:
		{
			Vector2 converMousePos = mousePos - m_Pos;
			float convertPos = converMousePos.y / m_Size.y;
			m_Percent = convertPos;
			SetPercent(m_Percent);
			m_Handle->SetPos(m_Handle->GetWindowPos().x, m_Pos.y + (m_Percent * m_Size.y) - m_Handle->GetWindowSize().y / 2.0f);
		}
			break;
		}
	}
}

void CSliderWidget::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
	m_Background->PostUpdate(deltaTime);
	m_Handle->PostUpdate(deltaTime);
}

void CSliderWidget::Render()
{
	if (m_Info.texture)
		m_Info.texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, 0);

	m_Tint = m_Info.tint;

	m_ProgressCBuffer->UpdateCBuffer();

	m_Background->Render();
	CWidget::Render();
	m_Handle->Render();
}

CSliderWidget* CSliderWidget::Clone()
{
	return new CSliderWidget(*this);
}
