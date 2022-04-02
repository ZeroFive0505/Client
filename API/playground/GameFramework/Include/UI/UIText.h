#pragma once

#include "UIWidget.h"
#include "../Resource/Font.h"

/// <summary>
/// 텍스트 출력을 하는 위젯
/// </summary>
class CUIText :
	public CUIWidget
{
public:
	CUIText();
	CUIText(const CUIText& widget);
	virtual ~CUIText();

protected:
	// 출력될 텍스트
	TCHAR* m_Text;
	// 텍스트의 수
	int m_TextCount;
	// 텍스트 가용량
	int m_TextCapacity;
	// 텍스트의 색깔
	COLORREF m_TextColor;
	// 텍스트의 폰트
	CSharedPtr<CFont> m_Font;

public:
	void SetTextColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_TextColor = RGB(r, g, b);
	}

	void SetText(const TCHAR* Text)
	{
		int Length = lstrlen(Text);

		// 만약 현재 텍스트보다 더 긴 텍스트로 설정된다면
		if (Length + 1 > m_TextCapacity)
		{
			// 새롭게 할당하고 복사한다.
			m_TextCapacity = Length + 1;

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = new TCHAR[m_TextCapacity];

			memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
		}

		lstrcpy(m_Text, Text);

		m_TextCount = lstrlen(Text);
	}

	// SetText와 동일한 기능
	void AddText(const TCHAR* Text)
	{
		int Length = m_TextCount + lstrlen(Text);

		if (Length + 1 > m_TextCapacity)
		{
			m_TextCapacity = Length + 1;

			TCHAR* Array = new TCHAR[m_TextCapacity];

			memset(Array, 0, sizeof(TCHAR) * m_TextCapacity);

			lstrcpy(Array, m_Text);

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = Array;
		}

		lstrcat(m_Text, Text);

		m_TextCount += lstrlen(Text);
	}

	void pop_back()
	{
		m_TextCount--;

		m_Text[m_TextCount] = 0;
	}

	// 텍스트를 전부 초기화 시킨다.
	void Clear()
	{
		m_TextCount = 0;

		memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	}

	void SetFont(const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CUIText* Clone();
};

