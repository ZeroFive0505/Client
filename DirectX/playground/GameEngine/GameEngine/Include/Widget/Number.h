#pragma once

#include "Widget.h"

class CNumber :
	public CWidget
{
	friend class CWidgetWindow;

protected:
	CNumber();
	CNumber(const CNumber& widget);
	virtual ~CNumber();

protected:
	sWidgetImageInfo m_Info;
	int m_Number;
	std::vector<int> m_vecNumber;
	
public:
	inline void SetNumber(int number)
	{
		m_Number = number;
	}

	inline void AddNumber(int number)
	{
		m_Number += number;
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
	bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);

	inline void SetTextureTint(const Vector4& tint)
	{
		m_Info.tint = tint;
	}

	inline void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		m_Info.tint = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void AddFrameData(const Vector2& start, const Vector2& size);
	void AddFrameData(int count);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CNumber* Clone();
};

