#pragma once

#include "UIWindow.h"

class CMode7UI :
	public CUIWindow
{
	friend class CScene;

private:
	CMode7UI();
	virtual ~CMode7UI();

private:
	class CUIText* m_Far;
	class CUIText* m_Near;
	class CUIText* m_FOV;
	class CUIText* m_Perspective;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
};

