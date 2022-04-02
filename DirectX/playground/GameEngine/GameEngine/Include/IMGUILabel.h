#pragma once

#include "IMGUIWidget.h"

class CIMGUILabel :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUILabel();
	virtual ~CIMGUILabel();

protected:
	ImVec2 m_Align;

public:
	inline void SetAlign(float x, float y)
	{
		m_Align = ImVec2(x, y);
	}

	inline void SetAlign(const Vector2& align)
	{
		m_Align.x = align.x;
		m_Align.y = align.y;
	}

	inline void SetAlign(const ImVec2& align)
	{
		m_Align = align;
	}

public:
	virtual bool Init();
	virtual void Render();
};

