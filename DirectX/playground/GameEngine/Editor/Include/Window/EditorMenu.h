#pragma once

#include "IMGUIWindow.h"
#include "../EditorInfo.h"

/// <summary>
/// ������Ʈ�� �����Ҽ� �ִ� â
/// ������ ������Ʈ�� ������Ʈ �̸��� �Է��ϰ� 
/// ������Ʈ Ÿ���� ���ؼ� ���� �߰��Ѵ�.
/// </summary>
class CEditorMenu :
	public CIMGUIWindow
{
public:
	CEditorMenu();
	virtual ~CEditorMenu();

private:
	class CIMGUIButton* m_ObjectCreateButton;
	class CIMGUITextInput* m_ObjectNameInput;
	class CIMGUIComboBox* m_ObjectCombo;

	class CIMGUIButton* m_ComponentCreateButton;
	class CIMGUITextInput* m_ComponentNameInput;
	class CIMGUIComboBox* m_ComponentCombo;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

public:
	void ObjectCreateButton();
	void ComponentCreateButton();
	void SaveScene();
	void LoadScene();
	void GamePlay();
};

