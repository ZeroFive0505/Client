#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"

class CDriverSelectTitle :
	public CUIWidget
{
public:
	CDriverSelectTitle();
	CDriverSelectTitle(const CDriverSelectTitle& widget);
	virtual ~CDriverSelectTitle();

protected:
	CSharedPtr<CTexture> m_BackGround;
	CSharedPtr<CTexture> m_Title;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CDriverSelectTitle* Clone();
};

