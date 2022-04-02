#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/ProgressBar.h"
#include "Widget/Image.h"

class CLoadingWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CLoadingWidget();
	CLoadingWidget(const CLoadingWidget& window);
	virtual ~CLoadingWidget();

private:
	CSharedPtr<CImage> m_Background;
	CSharedPtr<CProgressBar> m_LoadingBar;

public:
	inline void SetLoadingPercent(float percent)
	{
		m_LoadingBar->SetPercent(percent);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CLoadingWidget* Clone();
};

