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
	// OffsetX�� ������ â�� �������κ��� ������� ��ġ��ŭ ����߸� �Ŀ� �׷����� ���Ѵ�.
	float m_OffsetX;

	// ���� ��ġ�κ��� ������ �Ÿ��� �ǹ��Ѵ�.
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

