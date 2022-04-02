#include "Number.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

CNumber::CNumber() :
	m_Number(0)
{
}

CNumber::CNumber(const CNumber& widget) :
	CWidget(widget)
{
	m_Number = widget.m_Number;
}

CNumber::~CNumber()
{
}

bool CNumber::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return false;

		m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return false;

		m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

bool CNumber::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
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

bool CNumber::SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTexture(name, vecFileName, pathName))
			return false;

		m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
			return false;

		m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

bool CNumber::SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	if (m_Owner->GetViewport())
	{
		if (!m_Owner->GetViewport()->GetScene()->GetSceneResource()->LoadTextureFullPath(name, vecFullPath))
			return false;

		m_Info.texture = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
			return false;

		m_Info.texture = CResourceManager::GetInst()->FindTexture(name);
	}

	SetUseTexture(true);

	return true;
}

void CNumber::AddFrameData(const Vector2& start, const Vector2& size)
{
	sAnimationFrameData data;
	data.start = start;
	data.size = size;

	m_Info.vecFrameData.push_back(data);
}

void CNumber::AddFrameData(int count)
{
	if (m_Info.texture->GetImageType() == Image_Type::Atlas)
		return;

	for (int i = 0; i < count; i++)
	{
		sAnimationFrameData data;
		data.start = Vector2(0.0f, 0.0f);
		data.size = Vector2((float)m_Info.texture->GetWidth(i), (float)m_Info.texture->GetHeight(i));

		m_Info.vecFrameData.push_back(data);
	}
}

void CNumber::Start()
{
	CWidget::Start();
}

bool CNumber::Init()
{
	if (!CWidget::Init())
		return false;

	if (m_Owner->GetViewport())
		m_Shader = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindShader("NumberShader");
	else
		m_Shader = CResourceManager::GetInst()->FindShader("NumberShader");

	return true;
}

void CNumber::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CNumber::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CNumber::Render()
{
	int number = m_Number;

	m_vecNumber.clear();

	if (number == 0)
		m_vecNumber.push_back(0);
	else
	{
		std::stack<int> st;

		while (number)
		{
			st.push(number % 10);
			number /= 10;
		}

		while (!st.empty())
		{
			m_vecNumber.push_back(st.top());
			st.pop();
		}
	}

	// 숫자 위젯은 애니메이션이 존재하지않음
	m_CBuffer->SetAnimEnable(false);

	size_t count = m_vecNumber.size();

	for (size_t i = 0; i < count; i++)
	{
		if (m_Info.texture)
		{
			int frame = 0;

			switch (m_Info.texture->GetImageType())
			{
			case Image_Type::Atlas:
			{
				Vector2	startUV, endUV;

				Vector2	start = m_Info.vecFrameData[m_Info.frame].start;
				Vector2	frameSize = m_Info.vecFrameData[m_Info.frame].size;

				startUV = start /
					Vector2((float)m_Info.texture->GetWidth(), (float)m_Info.texture->GetHeight());

				endUV = (start + frameSize) /
					Vector2((float)m_Info.texture->GetWidth(), (float)m_Info.texture->GetHeight());

				m_CBuffer->SetStartUV(startUV);
				m_CBuffer->SetEndUV(endUV);
			}
				break;
			case Image_Type::Frame:
				frame = m_vecNumber[i];

				m_CBuffer->SetStartUV(Vector2(0.f, 0.f));
				m_CBuffer->SetEndUV(Vector2(1.f, 1.f));
				break;
			case Image_Type::Array:
				break;
			}

			m_Info.texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, frame);
		}

		m_Tint = m_Info.tint;

		m_RenderPos.x += (m_Size.x * (float)i);

		CWidget::Render();
	}
}

CNumber* CNumber::Clone()
{
	return new CNumber(*this);
}
