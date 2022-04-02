#pragma once

#include "IMGUIWidget.h"

class CIMGUIButton :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIButton();
	virtual ~CIMGUIButton();

protected:
	std::function<void()> m_ClickCallback;
	bool m_Click;

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetClickCallback(T* obj, void(T::* func)())
	{
		m_ClickCallback = std::bind(func, obj);
	}
};

