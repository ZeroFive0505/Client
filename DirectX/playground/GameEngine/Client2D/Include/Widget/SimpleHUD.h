#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Text.h"
#include "Widget/ProgressBar.h"
#include "Widget/SliderWidget.h"

class CSimpleHUD :
	public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CSimpleHUD();
	CSimpleHUD(const CSimpleHUD& window);
	virtual ~CSimpleHUD();

private:
	CSharedPtr<CButton> m_Button;
	CSharedPtr<CText> m_NameText;
	CSharedPtr<CProgressBar> m_HPBar;

public:
	void SetText(const TCHAR* text);
	void SetHPPercent(float percent);
	void SetHPDirection(ProgressBar_Direction direction);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CSimpleHUD* Clone();
};

