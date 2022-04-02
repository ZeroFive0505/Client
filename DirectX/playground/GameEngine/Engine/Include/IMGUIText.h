#pragma once

#include "IMGUIWidget.h"

class CIMGUIText :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIText();
	virtual ~CIMGUIText();

protected:
	char m_Text[1024];
	wchar_t m_wText[1024];
	char m_TextUTF8[1024];

public:
	void SetText(const char* text)
	{
		memset(m_wText, 0, sizeof(wchar_t) * 1024);
		memset(m_Text, 0, sizeof(char) * 1024);
		memset(m_TextUTF8, 0, sizeof(char) * 1024);

		strcpy_s(m_Text, text);

		int length = MultiByteToWideChar(CP_ACP, 0, text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, text, -1, m_wText, length);

		// UTF8로 변환한다. (한글 출력을 위해)
		length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, length, 0, 0);
	}

	void AddText(const char* text)
	{
		wchar_t wText[1024] = {};
		int length = MultiByteToWideChar(CP_ACP, -1, text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, text, -1, wText, length);

		strcat_s(m_Text, text);

		lstrcat(m_wText, wText);

		length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, length, 0, 0);
	}

public:
	virtual bool Init();
	virtual void Render();
};

