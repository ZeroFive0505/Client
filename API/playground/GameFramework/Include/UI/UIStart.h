#pragma once

#include "UIWindow.h"

/// <summary>
/// ��ŸƮ UI
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
	// ���� ��ư�� ������ ȣ��Ǵ� �ݹ� �Լ�
	void StartClick();
	// ���� ��ư�� ������ ȣ��Ǵ� �ݹ� �Լ�
	void ExitClick();
	void EditorClick();
};

