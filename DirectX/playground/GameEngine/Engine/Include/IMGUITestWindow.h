#pragma once

#include "IMGUIWindow.h"

class CIMGUITestWindow :
	public CIMGUIWindow
{
public:
	CIMGUITestWindow();
	virtual ~CIMGUITestWindow();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

public:
	void ClickButton1();
	void ClickButton2();

	void ChangeToKorean();
	void ChangeToEnglish();

	void SetRed();
	void SetGreen();
	void SetBlue();

	void ResetRed();
	void ResetGreen();
	void ResetBlue();
};

