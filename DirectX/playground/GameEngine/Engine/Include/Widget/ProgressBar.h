#pragma once

#include "Widget.h"
#include "../Resource/Shader/ProgressBarConstantBuffer.h"

class CProgressBar :
	public CWidget
{
	friend class CWidgetWindow;

protected:
	CProgressBar();
	CProgressBar(const CProgressBar& widget);
	virtual ~CProgressBar();

protected:
	sWidgetImageInfo m_Info;
	std::function<void()> m_MouseOnCallback;
	float m_Percent;
	ProgressBar_Direction m_Direction;
	CProgressBarConstantBuffer* m_ProgressCBuffer;
	bool m_StartMouseOn;

public:
	inline float GetPercent() const
	{
		return m_Percent;
	}

public:
	inline void SetPercent(float percent)
	{
		m_Percent = percent;

		if (m_Percent > 1.0f)
			m_Percent = 1.0f;
		else if (m_Percent < 0.0f)
			m_Percent = 0.0f;

		m_ProgressCBuffer->SetPercent(m_Percent);
	}

	inline void SetDirection(ProgressBar_Direction direction)
	{
		m_Direction = direction;
		m_ProgressCBuffer->SetDirection(m_Direction);
	}

	virtual void Enable(bool bEnable)
	{
		CWidget::Enable(bEnable);
	}

	inline void SetTexture(CTexture* texture)
	{
		m_Info.texture = texture;

		SetUseTexture(true);
	}

	inline Vector2 GetTextureSize() const
	{
		return Vector2((float)m_Info.texture->GetWidth(), (float)m_Info.texture->GetHeight());
	}

	bool SetTexture(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);

	inline void SetTextureTint(const Vector4& tint)
	{
		m_Info.tint = tint;
	}
	inline void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Info.tint = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void AddFrameData(const Vector2& start, const Vector2& size);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CProgressBar* Clone();

public:
	template <typename T>
	void SetMouseOnCallback(T* obj, void(T::* func)())
	{
		m_MouseOnCallback = std::bind(func, obj);
	}

	void SetMouseOnCallback(void (*func)())
	{
		m_MouseOnCallback = std::bind(func);
	}
};

