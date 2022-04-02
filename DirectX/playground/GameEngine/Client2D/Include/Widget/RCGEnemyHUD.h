#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CRCGEnemyHUD :
	public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CRCGEnemyHUD();
	CRCGEnemyHUD(const CRCGEnemyHUD& window);
	virtual ~CRCGEnemyHUD();

private:
	CSharedPtr<CImage> m_ExclamationMark;
	float m_ElapsedTime;
	bool m_ShowExclamation;
	
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CRCGEnemyHUD* Clone();

public:
	inline void ShowExclmationMark(bool show)
	{
		m_ExclamationMark->SetRender(show);
		m_ShowExclamation = true;
		m_ElapsedTime = 0.0f;
	}
};

