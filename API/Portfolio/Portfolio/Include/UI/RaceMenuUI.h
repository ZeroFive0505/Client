#pragma once

#include "UIWindow.h"

class CRaceMenuUI :
	public CUIWindow
{
	friend class CScene;

private:
	CRaceMenuUI();
	virtual ~CRaceMenuUI();

private:
	class CUISpriteText* m_CC50;
	class CUISpriteText* m_CC100;
	class CUISpriteText* m_CC150;
	class CUISpriteText* m_CC200;

public:
	virtual bool Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hDC) override;

public:
	Vector2 GetCC50Pos() const;

	Vector2 GetCC100Pos() const;

	Vector2 GetCC150Pos() const;

	Vector2 GetCC200Pos() const;

	inline void ToggleVisibility()
	{
		m_Visibility = !m_Visibility;
	}
};

