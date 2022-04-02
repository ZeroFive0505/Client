#pragma once

#include "IMGUIWidget.h"

class CIMGUIImage :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIImage();
	virtual ~CIMGUIImage();


protected:
	CSharedPtr<class CTexture> m_Texture;
	ImVec2 m_ImageStart;
	ImVec2 m_ImageEnd;
	ImVec4 m_Tint;
	ImVec4 m_BorderColor;

public:
	inline Vector2 GetImageStart()
	{
		return Vector2(m_ImageStart.x, m_ImageStart.y);
	}

	inline Vector2 GetImageEnd()
	{
		return Vector2(m_ImageEnd.x, m_ImageEnd.y);
	}

public:
	inline void SetImageStart(float x, float y)
	{
		m_ImageStart = ImVec2(x, y);
	}

	inline void SetImageEnd(float x, float y)
	{
		m_ImageEnd = ImVec2(x, y);
	}

	inline void SetTint(unsigned char r, unsigned char g, unsigned char b)
	{
		m_Tint = ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	}

	inline void SetBorderColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_BorderColor = ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	}

	inline void SetRedTint(unsigned char r)
	{
		m_Tint.x = r / 255.0f;
	}

	inline void SetGreenTint(unsigned char g)
	{
		m_Tint.y = g / 255.0f;
	}

	inline void SetBlueTint(unsigned char b)
	{
		m_Tint.z = b / 255.0f;
	}

public:
	void SetTexture(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	void SetTexture(const std::string& name);
	void SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
	void SetTexture(class CTexture* texture);

public:
	virtual bool Init();
	virtual void Render();
};

