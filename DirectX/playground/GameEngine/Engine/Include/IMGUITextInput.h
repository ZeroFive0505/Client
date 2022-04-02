#pragma once

#include "IMGUIWidget.h"

enum class ImGuiText_Type
{
	String,
	Int,
	Float
};

class CIMGUITextInput :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUITextInput();
	virtual ~CIMGUITextInput();

protected:
	char m_Text[1024];
	wchar_t m_wText[1024];
	char m_TextUTF8[1024];

	char m_HintText[1024];
	wchar_t m_wHintText[1024];
	char m_HintTextUTF8[1024];
	bool m_MultiLine;
	ImGuiInputTextFlags m_Flag;

	ImGuiText_Type m_TextType;

	int m_ValueInt;
	float m_ValueFloat;

	std::function<void()> m_InputCallback;

public:
	inline int GetValueInt() const
	{
		return m_ValueInt;
	}

	inline float GetValueFloat() const
	{
		return m_ValueFloat;
	}

	inline const wchar_t* GetText() const
	{
		return m_wText;
	}

	inline const char* GetTextUTF8() const
	{
		return m_TextUTF8;
	}

	inline const char* GetTextMultibyte() const
	{
		return m_Text;
	}

public:
	void SetText(const char* text)
	{
		memset(m_wText, 0, sizeof(wchar_t) * 1024);
		memset(m_Text, 0, sizeof(char) * 1024);
		memset(m_TextUTF8, 0, sizeof(char) * 1024);

		strcpy_s(m_Text, text);

		int length = MultiByteToWideChar(CP_ACP, 0, text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, text, -1, m_wText, length);

		length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, length, 0, 0);
	}

	void SetHintText(const char* text)
	{
		memset(m_wHintText, 0, sizeof(wchar_t) * 1024);
		memset(m_HintText, 0, sizeof(char) * 1024);
		memset(m_HintTextUTF8, 0, sizeof(char) * 1024);

		int length = MultiByteToWideChar(CP_ACP, 0, text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, text, -1, m_wHintText, length);

		length = WideCharToMultiByte(CP_UTF8, 0, m_wHintText, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, m_wHintText, -1, m_HintTextUTF8, length, 0, 0);
	}

	inline void SetInt(int value)
	{
		m_ValueInt = value;
	}

	inline void SetFloat(float value)
	{
		m_ValueFloat = value;
	}

	inline void SetMultiLine(bool multiLine)
	{
		m_MultiLine = multiLine;
	}

	inline void AddFlag(ImGuiInputTextFlags_ flag)
	{
		m_Flag |= flag;
	}

	inline void ReadOnly(bool enable)
	{
		if (enable)
			m_Flag |= ImGuiInputTextFlags_ReadOnly;
		else if (m_Flag & ImGuiInputTextFlags_ReadOnly)
			m_Flag ^= ImGuiInputTextFlags_ReadOnly;
	}

	void EnablePassword(bool enable)
	{
		if (enable)
			m_Flag |= ImGuiInputTextFlags_Password;
		else if (m_Flag & ImGuiInputTextFlags_Password)
			m_Flag ^= ImGuiInputTextFlags_Password;
	}
	
	inline void SetTextType(ImGuiText_Type type)
	{
		m_TextType = type;
	}

	inline bool Empty() const
	{
		return strlen(m_Text) == 0;
	}
	
public:
	virtual bool Init();
	virtual void Render();
	
public:
	template <typename T>
	void SetCallback(T* obj, void(T::* func)())
	{
		m_InputCallback = std::bind(func, obj);
	}
};

