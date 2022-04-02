#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"


class CTimerWidget :
	public CUIWidget
{
public:
	CTimerWidget();
	CTimerWidget(const CTimerWidget& widget);
	virtual ~CTimerWidget();

protected:
	float m_Time;

	float m_Factor;

	Vector2 m_Scale;

	Vector2 m_LeftTop;

	CSharedPtr<CTexture> m_Digits[10];
	CSharedPtr<CTexture> m_SingleQuote;
	CSharedPtr<CTexture> m_DoubleQuote;

	CSharedPtr<CTexture> m_Colon;
	CSharedPtr<CTexture> m_Bar;
	CSharedPtr<CTexture> m_Period;

	std::vector<Vector2> m_TimerPos;
	std::vector<Vector2> m_QuotePos;
	std::vector<int> m_TimerIdx;

public:
	void SetWindowSize();


public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CTimerWidget* Clone();
};

