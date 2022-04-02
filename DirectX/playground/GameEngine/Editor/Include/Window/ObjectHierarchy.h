#pragma once

#include "IMGUIWindow.h"
#include "GameObject/GameObject.h"
#include "Component/SceneComponent.h"


/// <summary>
/// ������Ʈ ���ϰ���� ������Ʈ ����� �����ִ� â
/// </summary>
class CObjectHierarchy :
	public CIMGUIWindow
{
public:
	CObjectHierarchy();
	virtual ~CObjectHierarchy();

private:
	class CIMGUIListBox* m_ObjectListWidget;
	class CIMGUIListBox* m_ComponentListWidget;
	CSharedPtr<CGameObject> m_SelectObject;
	CSharedPtr<CSceneComponent> m_SelectComponent;

public:
	inline class CIMGUIListBox* GetObjectList() const
	{
		return m_ObjectListWidget;
	}

	inline class CIMGUIListBox* GetComponentList() const
	{
		return m_ComponentListWidget;
	}

	inline CGameObject* GetSelectObject() const
	{
		return m_SelectObject;
	}

public:
	void AddObjectList(const char* name);
	void AddComponentList(const char* name);

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

private:
	void SelectObject(int index, const char* item);
	void SelectComponent(int index, const char* item);
};

