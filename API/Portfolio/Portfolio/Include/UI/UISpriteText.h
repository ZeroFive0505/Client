#pragma once

#include "UIWidget.h"


class CUISpriteText :
	public CUIWidget
{
public:
	CUISpriteText();
	CUISpriteText(const CUISpriteText& widget);
	virtual ~CUISpriteText();

private:
	std::vector<std::pair<std::string, Vector2>> m_SpriteText;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CUISpriteText* Clone();

public:
	void SetText(const std::string& text);
};

