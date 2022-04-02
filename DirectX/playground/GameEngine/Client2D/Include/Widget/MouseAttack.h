#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CMouseAttack :
	public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;
	friend class CEngine;

protected:
	CMouseAttack();
	CMouseAttack(const CMouseAttack& window);
	virtual ~CMouseAttack();

private:
	CSharedPtr<CImage> m_Image;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CMouseAttack* Clone();
};

