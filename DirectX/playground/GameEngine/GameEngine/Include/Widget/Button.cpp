#include "Button.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

CButton::CButton() :
	m_State(Button_State::Normal),
	m_MouseOnSound(false),
	m_ClickSound(false)
{
}

CButton::CButton(const CButton& widget) :
	CWidget(widget)
{
	m_State = Button_State::Normal;
}

CButton::~CButton()
{
}

bool CButton::SetTexture(Button_State state, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return false;

		m_InfoArr[(int)state].texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return false;

		m_InfoArr[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
	}


	SetUseTexture(true);

	return true;
}

bool CButton::SetTextureFullPath(Button_State state, const std::string& name, const TCHAR* fullPath)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, fullPath))
			return false;

		m_InfoArr[(int)state].texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
			return false;

		m_InfoArr[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

bool CButton::SetTexture(Button_State state, const std::string& name, const std::vector<TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, vecFileName, pathName))
			return false;

		m_InfoArr[(int)state].texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
			return false;

		m_InfoArr[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

bool CButton::SetTextureFullPath(Button_State state, const std::string& name,
	const std::vector<TCHAR*>& vecFullPath)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, vecFullPath))
			return false;

		m_InfoArr[(int)state].texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
			return false;

		m_InfoArr[(int)state].texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

void CButton::AddFrameData(Button_State state, const Vector2& start, const Vector2& size)
{
	sAnimationFrameData data;

	data.start = start;
	data.size = size;

	m_InfoArr[(int)state].vecFrameData.push_back(data);
}

void CButton::AddFrameData(Button_State state, int count)
{
	if (m_InfoArr[(int)state].texture->GetImageType() == Image_Type::Atlas)
		return;

	for (int i = 0; i < count; i++)
	{
		sAnimationFrameData data;

		data.start = Vector2(0.0f, 0.0f);
		data.size = Vector2((float)m_InfoArr[(int)state].texture->GetWidth(i),
			(float)m_InfoArr[(int)state].texture->GetHeight(i));
	}
}

void CButton::SetSound(Button_Sound_State state, const std::string& name)
{
	m_Sound[(int)state] = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindSound(name);
}

void CButton::SetSound(Button_Sound_State state, CSound* sound)
{
	m_Sound[(int)state] = sound;
}

void CButton::SetSound(Button_Sound_State state, const std::string& channelGroupName, const std::string& name, const char* fileName, const std::string& pathName)
{
	m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadSound(channelGroupName, false, name, fileName, pathName);

	m_Sound[(int)state] = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindSound(name);
}

void CButton::Start()
{
	CWidget::Start();
}

bool CButton::Init()
{
	if (!CWidget::Init())
		return false;

	return true;
}

void CButton::Update(float deltaTime)
{
	CWidget::Update(deltaTime);

	if (m_State != Button_State::Disable)
	{
		if (m_MouseHovered)
		{
			if (!m_MouseOnSound)
			{
				m_MouseOnSound = true;

				if (m_Sound[(int)Button_Sound_State::MouseOn])
					m_Sound[(int)Button_Sound_State::MouseOn]->Play();
			}


			if (CInput::GetInst()->GetMouseLButtonClick())
			{
				m_State = Button_State::Click;

				if (!m_ClickSound)
				{
					m_ClickSound = true;

					if (m_Sound[(int)Button_Sound_State::Click])
						m_Sound[(int)Button_Sound_State::Click]->Play();
				}
			}
			else if (m_State == Button_State::Click)
			{
				if (m_ClickCallback)
					m_ClickCallback();

				m_State = Button_State::MouseOn;
				m_ClickSound = false;
			}
			else
			{
				m_State = Button_State::MouseOn;
				m_ClickSound = false;
			}
		}
		else
		{
			m_State = Button_State::Normal;
			m_MouseOnSound = false;
			m_ClickSound = false;
		}
	}
}

void CButton::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);

	if (!m_InfoArr[(int)m_State].vecFrameData.empty())
	{
		m_InfoArr[(int)m_State].elapsedTime += deltaTime * m_InfoArr[(int)m_State].playScale;

		m_InfoArr[(int)m_State].frameTime = m_InfoArr[(int)m_State].playTime / m_InfoArr[(int)m_State].vecFrameData.size();

		if (m_InfoArr[(int)m_State].elapsedTime >= m_InfoArr[(int)m_State].frameTime)
		{
			m_InfoArr[(int)m_State].elapsedTime -= m_InfoArr[(int)m_State].frameTime;

			m_InfoArr[(int)m_State].frame = (m_InfoArr[(int)m_State].frame + 1) % m_InfoArr[(int)m_State].vecFrameData.size();
		}
	}
}

void CButton::Render()
{
	int frame = 0;

	if (!m_InfoArr[(int)m_State].vecFrameData.empty())
	{
		m_CBuffer->SetAnimEnable(true);
		m_CBuffer->SetAnimType((int)m_InfoArr[(int)m_State].texture->GetImageType());

		switch (m_InfoArr[(int)m_State].texture->GetImageType())
		{
		case Image_Type::Atlas:
		{
			Vector2 start = m_InfoArr[(int)m_State].vecFrameData[m_InfoArr[(int)m_State].frame].start;
			Vector2 size = m_InfoArr[(int)m_State].vecFrameData[m_InfoArr[(int)m_State].frame].size;

			Vector2 startUV = start / Vector2((float)m_InfoArr[(int)m_State].texture->GetWidth(),
				(float)m_InfoArr[(int)m_State].texture->GetHeight());

			Vector2 endUV = (start + size) / Vector2((float)m_InfoArr[(int)m_State].texture->GetWidth(),
				(float)m_InfoArr[(int)m_State].texture->GetHeight());

			m_CBuffer->SetStartUV(startUV);
			m_CBuffer->SetEndUV(endUV);
		}
			break;
		case Image_Type::Frame:
			frame = m_InfoArr[(int)m_State].frame;
			m_CBuffer->SetStartUV(Vector2(0.0f, 0.0f));
			m_CBuffer->SetEndUV(Vector2(1.0f, 1.0f));
			break;
		case Image_Type::Array:
			break;
		}
	}
	else
		m_CBuffer->SetAnimEnable(false);

	if (m_InfoArr[(int)m_State].texture)
		m_InfoArr[(int)m_State].texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, frame);

	m_Tint = m_InfoArr[(int)m_State].tint;

	CWidget::Render();
}

CButton* CButton::Clone()
{
	return new CButton(*this);
}
