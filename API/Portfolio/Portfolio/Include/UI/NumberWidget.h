#pragma once

#include "UIImage.h"
#include "../Resource/Texture.h"

class CNumberWidget :
	public CUIImage
{
public:
	CNumberWidget();
	CNumberWidget(const CNumberWidget& widget);
	virtual ~CNumberWidget();

protected:
	int m_Number;
	std::vector<int> m_vecNumber;

public:
	void SetNumber(int number)
	{
		m_Number = number;
	}

	void AddNumber(int number)
	{
		m_Number += number;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CNumberWidget* Clone();
};

