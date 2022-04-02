#pragma once

#include "UIWindow.h"
#include "DriverWidget.h"
#include "../Animation/DriverAnimator.h"
#include "../Resource/Texture.h"

class CDriverSelectUI :
	public CUIWindow
{
	friend class CScene;

private:
	CDriverSelectUI();
	virtual ~CDriverSelectUI();

private:
	class CUIImage* m_BackGround;
	class CUIImage* m_Confirm;
	class CUIImage* m_TitleBG;
	class CUIImage* m_Title;
	CDriverWidget* m_DriverWidgets[8];

	int m_Select;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

public:
	void Select(int select);
	void Cancel();

	inline void ResetConfirm(int select)
	{
		m_DriverWidgets[select]->ResetConfirm();
	}

	inline void Confirm(int select)
	{
		m_DriverWidgets[select]->Confirm();
	}

	inline bool IsSelected(int select)
	{
		return m_DriverWidgets[select]->HasBeenSelected();
	}
};

