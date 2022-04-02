#include "ObjectHierarchy.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUIManager.h"
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/SpriteComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "../Object/DragObject.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "TileMapWindow.h"

CObjectHierarchy::CObjectHierarchy() :
	m_ObjectListWidget(nullptr),
	m_ComponentListWidget(nullptr)
{
}

CObjectHierarchy::~CObjectHierarchy()
{
}

void CObjectHierarchy::AddObjectList(const char* name)
{
	m_ObjectListWidget->AddItem(name);
}

void CObjectHierarchy::AddComponentList(const char* name)
{
	m_ComponentListWidget->AddItem(name);
}

bool CObjectHierarchy::Init()
{
	CIMGUIWindow::Init();

	m_ObjectListWidget = AddWidget<CIMGUIListBox>("ObjectList", 300.0f, 500.0f);
	m_ObjectListWidget->SetHideName(true);
	m_ObjectListWidget->SetPageItemCount(15);
	m_ObjectListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectObject);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentListWidget = AddWidget<CIMGUIListBox>("ComponentList", 300.0f, 500.0f);
	m_ComponentListWidget->SetHideName(true);
	m_ComponentListWidget->SetPageItemCount(15);
	m_ComponentListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectComponent);

	return true;
}

void CObjectHierarchy::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CObjectHierarchy::SelectObject(int index, const char* item)
{
	CGameObject* object = CSceneManager::GetInst()->GetScene()->FindObject(item);

	if (!object)
		return;

	m_SelectObject = object;

	std::vector<sFindComponentName> vecNames;

	object->GetAllSceneComponentsName(vecNames);

	m_ComponentListWidget->Clear();

	size_t size = vecNames.size();

	for (size_t i = 0; i < size; i++)
	{
		m_ComponentListWidget->AddItem(vecNames[i].name);
	}
}

void CObjectHierarchy::SelectComponent(int index, const char* item)
{
	// 컴포넌트는 오브젝트에 소속되어 있으므로
	// 선택된 오브젝트가 없을시에는 반환
	if (!m_SelectObject)
		return;

	// 선택된 컴포넌트를 지정해둔다.
	m_SelectComponent = (CSceneComponent*)m_SelectObject->FindComponent(item);


	CTileMapWindow* tileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");

	if (tileMapWindow)
	{
		if (m_SelectComponent->TypeCheck<CTileMapComponent>())
		{
			tileMapWindow->SetTileMap((CTileMapComponent*)*m_SelectComponent);
			CEditorManager::GetInst()->SetEditMode(EditMode::TileMap);
		}
		else
			tileMapWindow->SetTileMap(nullptr);
	}

}
