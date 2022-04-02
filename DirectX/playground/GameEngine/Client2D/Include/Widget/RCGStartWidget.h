#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Button.h"

class CRCGStartWidget :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CRCGStartWidget();
	CRCGStartWidget(const CRCGStartWidget& widget);
	virtual ~CRCGStartWidget();

private:
	CSharedPtr<CImage> m_BackGround;
	CSharedPtr<CImage> m_Border;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CRCGStartWidget* Clone();

public:
	void StartKeyDown(float deltaTime);
	void ExitKeyDown(float deltaTime);
};

