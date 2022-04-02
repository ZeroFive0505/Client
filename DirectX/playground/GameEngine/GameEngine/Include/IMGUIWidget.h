#pragma once

#include "GameInfo.h"

// 위젯의 기본이 되는 클래스
class CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIWidget();
	virtual ~CIMGUIWidget();

protected:
	std::string m_Name;
	class CIMGUIWindow* m_Owner;
	ImVec2 m_Size;
	ImColor m_Color;
	ImFont* m_Font;
	bool m_Hide;

public:
	inline void SetOwner(class CIMGUIWindow* owner)
	{
		m_Owner = owner;
	}

	inline void SetName(const std::string& name)
	{
		m_Name = name;
	}

	inline void SetHideName(bool hide)
	{
		if (hide)
			m_Name = "##" + m_Name;

		m_Hide = hide;
	}

	inline bool IsHidden() const
	{
		return m_Hide;
	}

	inline void SetSize(const ImVec2& size)
	{
		m_Size = size;
	}

	inline void SetSize(const Vector2& size)
	{
		m_Size.x = size.x;
		m_Size.y = size.y;
	}

	inline void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	inline void SetColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a = 255)
	{
		m_Color.Value.x = r / 255.0f;
		m_Color.Value.y = g / 255.0f;
		m_Color.Value.z = b / 255.0f;
		m_Color.Value.w = a / 255.0f;
	}

public:
	virtual bool Init();
	virtual void Render();
};

