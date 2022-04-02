#include "IMGUIImage.h"
#include "Resource/Texture/Texture.h"
#include "Resource/ResourceManager.h"

CIMGUIImage::CIMGUIImage() :
	m_ImageStart{},
	m_ImageEnd{},
	m_Tint{ 1.0f, 1.0f, 1.0f, 1.0f },
	m_BorderColor{}
{
}

CIMGUIImage::~CIMGUIImage()
{
}


void CIMGUIImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CResourceManager::GetInst()->LoadTexture(name, fileName, pathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTexture(const std::string& name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTexture(CTexture* texture)
{
	m_Texture = texture;

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

bool CIMGUIImage::Init()
{
	SetTexture("DefaultUI", TEXT("DefaultUI.png"));

	SetImageStart(0.0f, 0.0f);
	SetImageEnd(100.0f, 100.0f);

	return true;
}

void CIMGUIImage::Render()
{
	if (m_Texture)
	{
		unsigned int width = m_Texture->GetWidth();
		unsigned int height = m_Texture->GetHeight();

		ImVec2 startUV = ImVec2(m_ImageStart.x / width, m_ImageStart.y / height);
		ImVec2 endUV = ImVec2(m_ImageEnd.x / width, m_ImageEnd.y / height);

		ImGui::Image(m_Texture->GetResource(), m_Size, startUV, endUV, m_Tint, m_BorderColor);
	}
}
