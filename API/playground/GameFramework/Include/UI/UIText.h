#pragma once

#include "UIWidget.h"
#include "../Resource/Font.h"

/// <summary>
/// �ؽ�Ʈ ����� �ϴ� ����
/// </summary>
class CUIText :
	public CUIWidget
{
public:
	CUIText();
	CUIText(const CUIText& widget);
	virtual ~CUIText();

protected:
	// ��µ� �ؽ�Ʈ
	TCHAR* m_Text;
	// �ؽ�Ʈ�� ��
	int m_TextCount;
	// �ؽ�Ʈ ���뷮
	int m_TextCapacity;
	// �ؽ�Ʈ�� ����
	COLORREF m_TextColor;
	// �ؽ�Ʈ�� ��Ʈ
	CSharedPtr<CFont> m_Font;

public:
	void SetTextColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_TextColor = RGB(r, g, b);
	}

	void SetText(const TCHAR* Text)
	{
		int Length = lstrlen(Text);

		// ���� ���� �ؽ�Ʈ���� �� �� �ؽ�Ʈ�� �����ȴٸ�
		if (Length + 1 > m_TextCapacity)
		{
			// ���Ӱ� �Ҵ��ϰ� �����Ѵ�.
			m_TextCapacity = Length + 1;

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = new TCHAR[m_TextCapacity];

			memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
		}

		lstrcpy(m_Text, Text);

		m_TextCount = lstrlen(Text);
	}

	// SetText�� ������ ���
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

	// �ؽ�Ʈ�� ���� �ʱ�ȭ ��Ų��.
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

