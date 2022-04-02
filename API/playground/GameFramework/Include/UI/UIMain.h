#pragma once

#include "UIWindow.h"

/// <summary>
/// 메인씬에 사용되는 UI
/// </summary>
class CUIMain :
	public CUIWindow
{
	friend class CScene;

private:
	CUIMain();
	virtual ~CUIMain();

private:
	// 대화창
	class CUIText* m_Text;
	float m_TextTime;

	int m_OutputText;
	int m_OutputIndex;

	// 분 초 위젯
	class CNumberWidget* m_NumberWidget;
	class CNumberWidget* m_Number1Widget;
	class CNumberWidget* m_MinuteWidget;

	float m_Time;
	int m_Minute;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

