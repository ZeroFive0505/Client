#pragma once

#include "UIWidget.h"

#include "../Resource/Texture.h"

class CRankWidget :
	public CUIWidget
{
public:
	CRankWidget();
	CRankWidget(const CRankWidget& widget);
	virtual ~CRankWidget();

protected:
	CSharedPtr<CTexture> m_Ranks[8];
	int m_CurrentRank;
	Vector2 m_Scale;


public:
	inline void SetRank(int rank)
	{
		m_CurrentRank = rank;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CRankWidget* Clone();
};

