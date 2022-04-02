#pragma once

#include "Widget.h"
#include "Image.h"
#include "../Resource/Shader/ProgressBarConstantBuffer.h"

class CSliderWidget :
	public CWidget
{
	friend class CWidgetWindow;

protected:
	CSliderWidget();
	CSliderWidget(const CSliderWidget& widget);
	virtual ~CSliderWidget();

private:
	sWidgetImageInfo m_Info;
	CSharedPtr<CImage> m_Background;
	CSharedPtr<CImage> m_Handle;
	std::function<void()> m_MouseOnCallback;
	float m_Percent;
	CProgressBarConstantBuffer* m_ProgressCBuffer;
	ProgressBar_Direction m_Direction;
	std::function<void()> m_MouseClickCallback;
	bool m_StartMouseOn;
	bool m_SliderWidgetDown;
	bool m_SliderWidgetHold;

public:
	inline float GetPercent() const
	{
		return m_Percent;
	}

	inline Vector2 GetBackgroundSize() const
	{
		return m_Background->GetWindowSize();
	}

	inline Vector2 GetHandleSize() const
	{
		return m_Handle->GetWindowSize();
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


	bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool SetTexture(const std::string& name, const TCHAR* fullPath);

	inline void SetTextureTint(const Vector4& tint)
	{
		m_Info.tint = tint;
	}

	inline void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Info.tint = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	inline void SetBGTextureTint(const Vector4& tint)
	{
		m_Background->SetTextureTint(tint);
	}

	inline void SetBGTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Background->SetTextureTint(r, g, b, a);
	}

	inline void SetBGTexturePos(const Vector2& pos)
	{
		m_Background->SetPos(pos);
	}

	inline void SetBGTexturePos(float x, float y)
	{
		m_Background->SetPos(x, y);
	}

	inline void SetBGTextureSize(const Vector2& size)
	{
		m_Background->SetSize(size);
	}

	inline void SetBGTextureSize(float x, float y)
	{
		m_Background->SetSize(x, y);
	}

	inline void SetHandlePos(const Vector2& pos)
	{
		m_Handle->SetPos(pos);
	}

	inline void SetHandlePos(float x, float y)
	{
		m_Handle->SetPos(x, y);
	}

	inline void SetHandleSize(const Vector2& size)
	{
		m_Handle->SetSize(size);
	}

	inline void SetHandleSize(float x, float y)
	{
		m_Handle->SetSize(x, y);
	}

	inline void SetHandleTextureTint(const Vector4& tint)
	{
		m_Handle->SetTextureTint(tint);
	}

	inline void SetHandleTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Handle->SetTextureTint(r, g, b, a);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CSliderWidget* Clone();

public:
	template <typename T>
	void SetMouseOnCallback(T* obj, void(T::* func)())
	{
		m_MouseOnCallback = std::bind(func, obj);
	}

	void SetMouseOnCallback(void(*func)())
	{
		m_MouseOnCallback = std::bind(func);
	}
};

