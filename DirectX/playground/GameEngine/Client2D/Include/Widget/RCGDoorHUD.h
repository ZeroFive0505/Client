#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/ProgressBar.h"
#include "Widget/Image.h"

class CRCGDoorHUD :
	public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CRCGDoorHUD();
	CRCGDoorHUD(const CRCGDoorHUD& window);
	virtual ~CRCGDoorHUD();

private:
	CSharedPtr<CImage> m_Progress;
	CSharedPtr<CImage> m_BackGround;

public:
	inline void SetProgress(float percent)
	{
		m_Progress->SetSize(100.0f, 100.0f * percent);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CRCGDoorHUD* Clone();
};

