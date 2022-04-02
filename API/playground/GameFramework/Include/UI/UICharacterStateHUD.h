#pragma once

#include "UIWindow.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"

/// <summary>
/// 캐릭터의 정보를 표현한다.
/// </summary>
class CUICharacterStateHUD :
	public CUIWindow
{
	friend class CScene;

private:
	CUICharacterStateHUD();
	~CUICharacterStateHUD();

private:
	// 프로그레스 바를 이용하여 채력을 표현한다.
	CSharedPtr<CProgressBar> m_HPBar;

public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}

public:
	virtual bool Init();
};

