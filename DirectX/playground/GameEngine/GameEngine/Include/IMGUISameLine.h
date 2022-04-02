#pragma once

#include "IMGUIWidget.h"

class CIMGUISameLine :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUISameLine();
	virtual ~CIMGUISameLine();

protected:
	// OffsetX는 윈도우 창의 왼쪽으로부터 어느정도 위치만큼 떨어뜨린 후에 그려낼지 정한다.
	float m_OffsetX;

	// 시작 위치로부터 떨어질 거리를 의미한다.
	float m_Spacing;

public:
	inline void SetOffsetX(float offsetX)
	{
		m_OffsetX = offsetX;
	}

	inline void SetSpacing(float spacing)
	{
		m_Spacing = spacing;
	}

public:
	virtual bool Init();
	virtual void Render();
};

