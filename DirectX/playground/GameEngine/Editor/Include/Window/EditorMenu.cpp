
#include "EditorMenu.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "../Object/DragObject.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "../EditorManager.h"
#include "ObjectHierarchy.h"
#include "IMGUIManager.h"
#include "DetailWindow.h"
#include "../Object/Player2D.h"


#include "GameObject/GameObject.h"


#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/TileMapComponent.h"

CEditorMenu::CEditorMenu()
{
}

CEditorMenu::~CEditorMenu()
{
}

bool CEditorMenu::Init()
{
	CIMGUIWindow::Init();

	m_ObjectCombo = AddWidget<CIMGUIComboBox>("ObjectCombo", 100.0f, 30.0f);

	m_ObjectCombo->SetHideName(true);
	m_ObjectCombo->AddItem("GameObject");
	m_ObjectCombo->AddItem("Player");

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("ObjectName", 100.0f, 30.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectCreateButton = AddWidget<CIMGUIButton>("CreateObject", 100.0f, 30.0f);

	m_ObjectCreateButton->SetClickCallback(this, &CEditorMenu::ObjectCreateButton);

	// Component
	m_ComponentCombo = AddWidget<CIMGUIComboBox>("ComponentCombo", 100.0f, 30.0f);
	m_ComponentCombo->SetHideName(true);

	m_ComponentCombo->AddItem("SpriteComponent");
	m_ComponentCombo->AddItem("StaticMeshComponent");
	m_ComponentCombo->AddItem("Box2D Collider");
	m_ComponentCombo->AddItem("Circle Collider");
	m_ComponentCombo->AddItem("Pixel Collider");
	m_ComponentCombo->AddItem("Camera Component");
	m_ComponentCombo->AddItem("Widget Component");
	m_ComponentCombo->AddItem("Particle Component");
	m_ComponentCombo->AddItem("TileMap Component");

	line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 100.0f, 30.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentCreateButton = AddWidget<CIMGUIButton>("CreateComponent", 100.f, 30.0f);

	m_ComponentCreateButton->SetClickCallback(this, &CEditorMenu::ComponentCreateButton);

	CIMGUIButton* saveSceneButton = AddWidget<CIMGUIButton>("SaveScene", 100.0f, 30.0f);

	saveSceneButton->SetClickCallback(this, &CEditorMenu::SaveScene);

	CIMGUIButton* loadSceneButton = AddWidget<CIMGUIButton>("LoadScene", 100.0f, 30.0f);

	loadSceneButton->SetClickCallback(this, &CEditorMenu::LoadScene);

	CIMGUIButton* playButton = AddWidget<CIMGUIButton>("Play", 50.0f, 50.0f);

	playButton->SetClickCallback(this, &CEditorMenu::GamePlay);

	return true;
}

void CEditorMenu::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CEditorMenu::ObjectCreateButton()
{
	// Object Create
	int selectIdx = m_ObjectCombo->GetSelectIndex();

	if (selectIdx == -1)
		return;

	if (m_ObjectNameInput->Empty())
		return;
		
	std::vector<sFindComponentName> vecComponents;

	switch ((CreateObject_Type)selectIdx)
	{
	case CreateObject_Type::GameObject:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Player:
		CPlayer2D* player = CSceneManager::GetInst()->GetScene()->CreateGameObject<CPlayer2D>(m_ObjectNameInput->GetTextMultibyte());


		player->GetAllSceneComponentsName(vecComponents);

		break;
	}

	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	// 계층에 아이템을 추가한다.
	if (hierarchy)
	{
		hierarchy->AddObjectList(m_ObjectNameInput->GetTextMultibyte());

		if (!vecComponents.empty())
		{
			size_t size = vecComponents.size();

			for (size_t i = 0; i < size; i++)
			{
				hierarchy->AddComponentList(vecComponents[i].name.c_str());
			}
		}
	}
}

void CEditorMenu::ComponentCreateButton()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	CGameObject* obj = nullptr;

	if (hierarchy)
	{
		// 오브젝트에 컴포넌트를 추가할시에는 오브젝트 리스트도 같이 받아온다.
		CIMGUIListBox* objList = hierarchy->GetObjectList();

		// 현재 선택된 아이템이 있는지 확인한다.
		if (objList->GetSelectIndex() == -1)
			return;

		// 오브젝트를 찾아온다.
		obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());
	}

	if (!obj)
		return;

	// 컴포넌트를 생성
	int selectIdx = m_ComponentCombo->GetSelectIndex();

	if (selectIdx == -1)
		return;

	if (m_ComponentNameInput->Empty())
		return;

	switch ((SceneComponent_Type)selectIdx)
	{
	case SceneComponent_Type::Sprite:
		obj->CreateComponent<CSpriteComponent>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::StaticMesh:
		obj->CreateComponent<CStaticMeshComponent>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::Box2DCollider:
		obj->CreateComponent<CColliderBox2D>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::CircleCollider:
		obj->CreateComponent<CColliderCircle>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::PixelCollider:
		obj->CreateComponent<CColliderPixel>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::Camera:
		obj->CreateComponent<CCameraComponent>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::Widget:
		obj->CreateComponent<CWidgetComponent>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::Particle:
		obj->CreateComponent<CParticleComponent>(m_ComponentNameInput->GetTextMultibyte());
		break;
	case SceneComponent_Type::TileMap:
	{
		CTileMapComponent* tileMap = obj->CreateComponent<CTileMapComponent>(m_ComponentNameInput->GetTextMultibyte());
		CMaterial* material = CSceneManager::GetInst()->GetScene()->GetSceneResource()->FindMaterial("TileMap");
		tileMap->SetTileMaterial(material);
	}
		break;
	}

	if (hierarchy)
	{
		CIMGUIListBox* compoentList = hierarchy->GetComponentList();

		compoentList->AddItem(m_ComponentNameInput->GetTextMultibyte());

		CDetailWindow* detailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");

		detailWindow->GetScaleXInput()->SetFloat(obj->GetWorldScale().x);
		detailWindow->GetScaleYInput()->SetFloat(obj->GetWorldScale().y);
		detailWindow->GetScaleZInput()->SetFloat(obj->GetWorldScale().z);
	}
}

void CEditorMenu::SaveScene()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("All files\0*.*\0Scene files\0*.scn");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->path;

	if (GetSaveFileName(&openFile) != 0)
	{
		char convertFullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		CSceneManager::GetInst()->GetScene()->SaveFullPath(convertFullPath);
	}
}

void CEditorMenu::LoadScene()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("All files\0*.*\0Scene files\0*.scn");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->path;

	if (GetOpenFileName(&openFile) != 0)
	{
		char convertFullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		CSceneManager::GetInst()->GetScene()->LoadFullPath(convertFullPath);

		const std::list<CSharedPtr<CGameObject>> objList = CSceneManager::GetInst()->GetScene()->GetAllObjects();

		CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

		if (hierarchy)
		{
			auto iter = objList.begin();
			auto iterEnd = objList.end();

			for (; iter != iterEnd; iter++)
			{
				std::vector<sFindComponentName> vecName;

				(*iter)->GetAllSceneComponentsName(vecName);

				hierarchy->AddObjectList((*iter)->GetName().c_str());


				size_t size = vecName.size();

				for (size_t i = 0; i < size; i++)
				{
					hierarchy->AddComponentList(vecName[i].name.c_str());
				}

			}
		}
	}
}

void CEditorMenu::GamePlay()
{
	if (!CEngine::GetInst()->IsPlay())
		CEngine::GetInst()->SetPlay(true);
	else
		CEngine::GetInst()->SetPlay(false);
}
