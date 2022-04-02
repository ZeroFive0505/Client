#pragma once

#include "Widget.h"

enum class TEXT_ALIGN_H
{
	Left,
	Center,
	Right
};

enum class TEXT_ALIGN_V
{
	Top,
	Middle,
	Bottom
};

class CText :
	public CWidget
{
	friend class CWidgetWindow;

protected:
	CText();
	CText(const CText& widget);
	virtual ~CText();

protected:
	TCHAR* m_Text;
	int m_TextCount;
	int m_TextCapacity;

protected:
	IDWriteTextFormat* m_Font;
	IDWriteTextLayout* m_Layout;
	ID2D1SolidColorBrush* m_ColorBrush;
	ID2D1RenderTarget* m_2DTarget;
	std::string m_FontKey;
	TCHAR* m_FontName;
	float m_FontSize;
	bool m_Alpha;
	float m_Opacity;
	Vector4 m_Color;


	bool m_ShadowEnable;
	ID2D1SolidColorBrush* m_ShadowColorBrush;
	bool m_ShadowAlpha;
	float m_ShadowOpacity;
	Vector4 m_ShadowColor;
	Vector2 m_ShadowOffset;

	TEXT_ALIGN_H m_AlignH;
	TEXT_ALIGN_V m_AlignV;

public:
	inline const TCHAR* GetText() const
	{
		return m_Text;
	}

	inline int GetTextCount() const
	{
		return m_TextCount;
	}

	inline virtual void SetSize(const Vector2& size)
	{
		CWidget::SetSize(size);
		CreateTextLayout();
	}

	inline virtual void SetSize(float x, float y)
	{
		CWidget::SetSize(x, y);
		CreateTextLayout();
	}


public:
	void SetText(const TCHAR* text)
	{
		int length = lstrlen(text);

		if (length + 1 > m_TextCapacity)
		{
			m_TextCapacity = length + 1;

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = new TCHAR[m_TextCapacity];

			memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
		}

		lstrcpy(m_Text, text);
		m_TextCount = length;

		// 문자가 추가되거나 바뀌면 새롭게 레이아웃을 생성한다.
		CreateTextLayout();
	}

	void AddText(const TCHAR* text)
	{
		int length = m_TextCount + lstrlen(text);

		if (length + 1 > m_TextCapacity)
		{
			m_TextCapacity = length * 2;

			TCHAR* temp = new TCHAR[m_TextCapacity];

			memset(temp, 0, sizeof(TCHAR) * m_TextCapacity);

			lstrcpy(temp, m_Text);

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = temp;
		}

		lstrcpy(m_Text, text);
		m_TextCount += lstrlen(text);

		CreateTextLayout();
	}

	void pop_back()
	{
		if (m_TextCount)
		{
			m_TextCount--;

			m_Text[m_TextCount] = 0;

			CreateTextLayout();
		}
	}

	void clear()
	{
		if (m_TextCount > 0)
		{
			m_TextCount = 0;

			memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

			CreateTextLayout();
		}
	}

public:
	void SetFont(const std::string& name);
	void SetFontSize(float size);
	void SetAlignH(TEXT_ALIGN_H align);
	void SetAlignV(TEXT_ALIGN_V align);
	void SetColor(float r, float g, float b);
	void SetColor(unsigned char r, unsigned char g, unsigned char b);
	void SetColor(const Vector4& color);
	void SetColor(unsigned int color);
	void SetAlphaEnable(bool alphaEnable);
	void SetOpacity(float opacity);

	void SetShadowEnable(bool shadowEnable);
	void SetShadowOffset(const Vector2& offset);
	void SetShadowOffset(float x, float y);
	void SetShadowColor(float r, float g, float b);
	void SetShadowColor(unsigned char r, unsigned char g, unsigned char b);
	void SetShadowColor(const Vector4& color);
	void SetShadowColor(unsigned int color);
	void SetShadowAlphaEnable(bool alphaEnable);
	void SetShadowOpacity(float opacity);

	bool CreateTextLayout();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
};

