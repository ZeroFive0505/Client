#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"

class CUIImage :
	public CUIWidget
{
public:
	CUIImage();
	CUIImage(const CUIImage& widget);
	virtual ~CUIImage();

protected:
	CSharedPtr<CTexture> m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;
	int m_FrameIndex;
	float m_PlayTime;
	float m_AnimTime;
	Vector2 m_Offset;

public:
	inline void SetTexture(CTexture* texture)
	{
		m_Texture = texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}

	inline void SetScale(float x, float y)
	{
		if (m_Texture)
		{
			m_Texture->SetScale(x, y);
		}
	}

	inline void SetScale(const Vector2& scale)
	{
		if (m_Texture)
		{
			m_Texture->SetScale(scale);
		}
	}

	inline Vector2 GetScale(int index = 0)
	{
		if (m_Texture)
		{
			return m_Texture->GetScale(index);
		}

		return Vector2(0.0f, 0.0f);
	}

	void SetTexture(const std::string& name);
	void SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
	void SetTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int index = 0);
	
	inline void AddFrameData(const Vector2& pos, const Vector2& size)
	{
		AnimationFrameData data = {};
		data.startPos = pos;
		data.size = size;

		m_vecFrameData.push_back(data);
	}

public:
	inline void Move(float x, float y)
	{
		m_Pos += Vector2(x, y);
	}

	inline void Move(const Vector2& move)
	{
		m_Pos += move;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CUIImage* Clone();
};

