#pragma once

#include "UIWindow.h"

/// <summary>
/// 스타트 UI
/// </summary>
class CUIStart : 
	public CUIWindow
{
	friend class CScene;

private:
	CUIStart();
	virtual ~CUIStart();

public:
	virtual bool Init();

public:
	// 시작 버튼을 누를시 호출되는 콜백 함수
	void StartClick();
	// 종료 버튼을 누를시 호출되는 콜백 함수
	void ExitClick();
	void EditorClick();
};

