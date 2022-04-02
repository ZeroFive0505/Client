#pragma once

#include "Widget.h"

class CImage :
	public CWidget
{
	friend class CWidgetWindow;
	friend class CSliderWidget;

protected:
	CImage();
	CImage(const CImage& widget);
	virtual ~CImage();

protected:
	sWidgetImageInfo m_Info;
	bool m_PlayOnce;
	bool m_Loop;
	bool m_Render;
	bool m_AnimStart;
	std::function<void()> m_ClickCallback;

public:
	virtual void Enable(bool bEnable)
	{
		CWidget::Enable(bEnable);
	}

	inline void SetTexture(CTexture* texture)
	{
		m_Info.texture = texture;

		SetUseTexture(true);
	}

	inline void SetPlayTime(float playTime)
	{
		m_Info.playTime = playTime;
	}

	inline void SetPlayScale(float playScale)
	{
		m_Info.playScale = playScale;
	}

	inline void SetPlayOnce(bool once)
	{
		m_PlayOnce = once;
	}

	inline void SetAnimStart(bool start)
	{
		m_AnimStart = start;
	}

	bool SetTexture(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& name,
		const TCHAR* fullPath);

	bool SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);

	inline void SetTextureTint(const Vector4& tint)
	{
		m_Info.tint = tint;
	}

	void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Info.tint = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void AddFrameData(const Vector2& start, const Vector2& size);

	void AddFrameData(int count);

public:
	inline Vector2 GetTextureSize(int index = 0) const
	{
		return Vector2((float)m_Info.texture->GetWidth(), (float)m_Info.texture->GetHeight());
	}

	inline bool IsLooping() const
	{
		return m_Loop;
	}

	inline void SetRender(bool render)
	{
		m_Render = render;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CImage* Clone();

public:
	template <typename T>
	void SetClickCallback(T* obj, void(T::* func)())
	{
		m_ClickCallback = std::bind(func, obj);
	}

	void SetClickCallback(void(*func)())
	{
		m_ClickCallback = std::bind(func);
	}
};

