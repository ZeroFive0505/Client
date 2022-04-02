#pragma once

#include "IMGUIWindow.h"
#include "../EditorInfo.h"

/// <summary>
/// 오브젝트를 생성할수 있는 창
/// 생성시 오브젝트와 컴포넌트 이름을 입력하고 
/// 컴포넌트 타입을 정해서 씬에 추가한다.
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

