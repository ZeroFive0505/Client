#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Text.h"
#include "Widget/SliderWidget.h"
#include "Widget/Number.h"

class CMainWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CMainWidget();
	CMainWidget(const CMainWidget& window);
	virtual ~CMainWidget();

private:
	CSharedPtr<CButton> m_Button;

	CSharedPtr<CButton> m_Button1;
	CSharedPtr<CText> m_Button1Text;

	CSharedPtr<CSliderWidget> m_SliderWidget;

	CSharedPtr<CNumber> m_Hour;
	CSharedPtr<CNumber> m_Minute;
	CSharedPtr<CNumber> m_Second;

	CSharedPtr<CText> m_FPSText;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
};

