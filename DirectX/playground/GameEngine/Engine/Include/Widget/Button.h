#pragma once

#include "Widget.h"

class CButton :
	public CWidget
{
	friend class CWidgetWindow;
	friend class CSliderWidget;

protected:
	CButton();
	CButton(const CButton& widget);
	virtual ~CButton();

protected:
	Button_State m_State;
	sWidgetImageInfo m_InfoArr[(int)Button_State::Max];
	std::function<void()> m_ClickCallback;
	CSharedPtr<class CSound> m_Sound[(int)Button_Sound_State::Max];
	bool m_MouseOnSound;
	bool m_ClickSound;

public:
	virtual void Enable(bool enable)
	{
		CWidget::Enable(enable);
	}

	inline void ButtonEnable(bool enable)
	{
		m_State = enable ? Button_State::Normal : Button_State::Disable;
	}

	inline void SetTexture(Button_State state, CTexture* texture)
	{
		m_InfoArr[(int)state].texture = texture;
	}

public:
	bool SetTexture(Button_State state, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	bool SetTextureFullPath(Button_State state, const std::string& name, const TCHAR* fullPath);
	bool SetTexture(Button_State state, const std::string& name, const std::vector<TCHAR*>& vecFileName, 
		const std::string& pathName = TEXTURE_PATH);
	bool SetTextureFullPath(Button_State state, const std::string& name, const std::vector<TCHAR*>& vecFullPath);

	inline void SetTextureTint(Button_State state, const Vector4& tint)
	{
		m_InfoArr[(int)state].tint = tint;
	}

	inline void SetTextureTint(Button_State state, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_InfoArr[(int)state].tint = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void AddFrameData(Button_State state, const Vector2& start, const Vector2& size);

	void AddFrameData(Button_State state, int count);

public:
	inline void SetPlayTime(Button_State state, float playTime)
	{
		m_InfoArr[(int)state].playTime = playTime;
	}

	inline void SetPlayScale(Button_State state, float playScale)
	{
		m_InfoArr[(int)state].playScale = playScale;
	}

public:
	void SetSound(Button_Sound_State state, const std::string& name);
	void SetSound(Button_Sound_State state, class CSound* sound);
	void SetSound(Button_Sound_State state, const std::string& channelGroupName,
		const std::string& name, const char* fileName,
		const std::string& pathName = SOUND_PATH);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CButton* Clone();

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

