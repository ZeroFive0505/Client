#pragma once

#include "UIWidget.h"

#include "../Resource/Texture.h"
#include "../Animation/DriverAnimator.h"

class CDriverWidget :
	public CUIWidget
{
public:
	CDriverWidget();
	CDriverWidget(const CDriverWidget& widget);
	virtual ~CDriverWidget();

private:
	CDriverAnimator m_Animator;

	CSharedPtr<CTexture> m_BG;

	float m_Angle;

	bool m_Selected;

	bool m_Confirm;

	float m_fElapsedTime;

public:
	virtual bool Init();
	void AnimatorInit(const std::string& name);
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CDriverWidget* Clone();

public:
	inline void Selected()
	{
		m_Selected = true;
	}

	inline void Confirm()
	{
		m_Confirm = true;
	}

	inline void ResetConfirm()
	{
		m_Confirm = false;
	}

	inline void Cancel()
	{
		m_Selected = false;
	}

	inline bool HasBeenSelected() const
	{
		return m_Selected;
	}
};

