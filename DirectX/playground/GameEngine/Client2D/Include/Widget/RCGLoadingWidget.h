#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CRCGLoadingWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CRCGLoadingWidget();
	CRCGLoadingWidget(const CRCGLoadingWidget& window);
	virtual ~CRCGLoadingWidget();

private:
	CSharedPtr<CImage> m_BackGround;
	CSharedPtr<CImage> m_LoadingAnim;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CRCGLoadingWidget* Clone();
};

