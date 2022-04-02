#pragma once

#include "UIWindow.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"

/// <summary>
/// ĳ������ ������ ǥ���Ѵ�.
/// </summary>
class CUICharacterStateHUD :
	public CUIWindow
{
	friend class CScene;

private:
	CUICharacterStateHUD();
	~CUICharacterStateHUD();

private:
	// ���α׷��� �ٸ� �̿��Ͽ� ä���� ǥ���Ѵ�.
	CSharedPtr<CProgressBar> m_HPBar;

public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}

public:
	virtual bool Init();
};

