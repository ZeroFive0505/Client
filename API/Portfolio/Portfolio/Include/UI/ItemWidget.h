#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"
#include "../Object/ObjectInfo.h"

class CItemWidget :
	public CUIWidget
{
public:
	CItemWidget();
	CItemWidget(const CItemWidget& widget);
	virtual ~CItemWidget();

protected:
	CSharedPtr<CTexture> m_ItemList[9];
	CSharedPtr<CTexture> m_CurrentItem;

	int m_SeletedItem;
	int m_FinalItem;

	float m_Time;
	bool m_Spining;
	bool m_ForceStop;

	Vector2 m_Scale;

public:
	void SetItem(EItem item);

	inline bool CanUse()
	{
		if (m_Spining)
			return false;
		else
			return true;
	}

	inline void SetForceStop()
	{
		m_ForceStop = true;
	}


public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& pos, HDC hDC);
	virtual CItemWidget* Clone();
};

