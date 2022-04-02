#pragma once

#include "UIWindow.h"

class CTitleUI :
	public CUIWindow
{
	friend class CScene;

private:
	CTitleUI();
	virtual ~CTitleUI();
	
private:
	class CUISpriteText* m_Race;
	class CUISpriteText* m_Tool;
	class CUISpriteText* m_Mode7;
	class CUISpriteText* m_Exit;

private:
	class CUIImage* m_TitleImg;

	bool m_Show;

public:
	virtual bool Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hDC) override;

public:
	inline void ToggleShow()
	{
		m_Show = !m_Show;
	}

	Vector2 GetRaceTextPos();

	Vector2 GetToolTextPos();

	Vector2 GetMode7TextPos();

	Vector2 GetExitTextPos();
};

