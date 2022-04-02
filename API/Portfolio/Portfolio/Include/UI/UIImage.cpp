#include "UIImage.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CUIImage::CUIImage() :
	m_FrameIndex(0),
	m_PlayTime(1.0f),
	m_AnimTime(0.0f)
{
}

CUIImage::CUIImage(const CUIImage& widget) :
	CUIWidget(widget)
{
	m_Texture = widget.m_Texture;
	m_PlayTime = widget.m_PlayTime;
	m_AnimTime = 0.0f;
	m_FrameIndex = 0;
	m_vecFrameData = widget.m_vecFrameData;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(const std::string& name)
{
	m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_Scene->GetSceneResoruce()->LoadTexture(name, fileName, pathName);

	m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	m_Scene->GetSceneResoruce()->LoadTextureFullPath(name, fullPath);

	m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
	m_Scene->GetSceneResoruce()->LoadTexture(name, vecFileName, pathName);

	m_Texture = m_Scene->GetSceneResoruce()->FindTexture(name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, index);
}

bool CUIImage::Init()
{
	return true;
}

void CUIImage::Update(float deltaTime)
{
	if (!m_vecFrameData.empty())
	{
		m_AnimTime += deltaTime;

		float frameTime = m_PlayTime / m_vecFrameData.size();

		if (m_AnimTime >= frameTime)
		{
			m_AnimTime -= frameTime;

			m_FrameIndex = (m_FrameIndex + 1) % m_vecFrameData.size();
		}
	}
}

void CUIImage::PostUpdate(float deltaTime)
{
}

void CUIImage::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2 pos = m_Pos + m_Owner->GetPos();
		Vector2 imagePos;
		Vector2 size = m_Size;

		if (!m_vecFrameData.empty())
		{
			imagePos = m_vecFrameData[m_FrameIndex].startPos;
			size = m_vecFrameData[m_FrameIndex].size;

			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size, m_FrameIndex);
			}
			else
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size);
			}
		}
		else
		{
			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size, m_FrameIndex);
			}
			else
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size);
			}
		}
	}
}

void CUIImage::Render(const Vector2& pos, HDC hDC)
{
	if (m_Texture)
	{
		Vector2 imagePos;
		Vector2 size;

		if (!m_vecFrameData.empty())
		{
			imagePos = m_vecFrameData[m_FrameIndex].startPos;
			size = m_vecFrameData[m_FrameIndex].size;

			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size, m_FrameIndex);
			}
			else
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size);
			}
		}
		else
		{
			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size, m_FrameIndex);
			}
			else
			{
				m_Texture->Render(hDC, pos + m_Offset, imagePos, size);
			}
		}
	}
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
