#pragma once

#include "UIWidget.h"
#include "../Resource/Font.h"

class CUIText :
	public CUIWidget
{
public:
	CUIText();
	CUIText(const CUIText& widget);
	virtual ~CUIText();

protected:
	TCHAR* m_Text;
	int m_TextCount;
	int m_TextCapacity;
	COLORREF m_TextColor;
	CSharedPtr<CFont> m_Font;

public:
	void SetTextColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_TextColor = RGB(r, g, b);
	}

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

		m_TextCount = lstrlen(text);
	}

	void AddText(const TCHAR* text)
	{
		int length = m_TextCount + lstrlen(text);

		if (length + 1 > m_TextCapacity)
		{
			m_TextCapacity = length + 1;

			TCHAR* arr = new TCHAR[m_TextCapacity];

			memset(arr, 0, sizeof(TCHAR) * m_TextCapacity);

			lstrcpy(arr, m_Text);

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = arr;
		}

		lstrcat(m_Text, text);

		m_TextCount += lstrlen(text);
	}

	inline void PopText()
	{
		m_TextCount--;

		m_Text[m_TextCount] = 0;
	}

	inline void Clear()
	{
		m_TextCount = 0;

		memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	}

	void SetFont(const std::string& name);

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CUIText* Clone();
};

