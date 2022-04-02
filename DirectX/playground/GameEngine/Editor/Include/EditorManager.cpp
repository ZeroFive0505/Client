#include "EditorManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Component/CameraComponent.h"
#include "Scene/DefaultScene.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Window/SpriteWindow.h"
#include "Object/DragObject.h"
#include "Render/RenderManager.h"
#include "Window/DetailWindow.h"
#include "Window/EditorMenu.h"
#include "Window/ObjectHierarchy.h"
#include "Window/TileMapWindow.h"
#include "Object/SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Object/Player2D.h"


DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager() :
	m_EditMode(EditMode::Scene),
	m_DragObj(nullptr),
	m_SpriteWindow(nullptr),
	m_DetailWindow(nullptr),
	m_EditorMenu(nullptr),
	m_ObjectHierarchy(nullptr),
	m_DragStart(false),
	m_DragEnd(false)
{

}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

void CEditorManager::SetEditMode(EditMode mode)
{
	m_EditMode = mode;

	switch (mode)
	{
	case EditMode::Scene:
		break;
	case EditMode::Sprite:
	{
		if (m_DragObj)
			m_DragObj->Destroy();
		m_DragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		m_DragObj->SetWorldScale(0.0f, 0.0f, 1.0f);
	}
		break;
	case EditMode::TileMap:
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}
	}
		break;
	}
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"), 1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CEditorManager>(this, &CEditorManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CEditorManager>(this, &CEditorManager::CreateComponet);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CEditorManager>(this, &CEditorManager::CreateAnimInstance);

	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->CreateKey("MouseRButton", VK_RBUTTON);

	CInput::GetInst()->CreateKey("AddFrameShortCut", 'V');
	CInput::GetInst()->CreateKey("DeleteLastFrameShortCut", 'X');

	CInput::GetInst()->CreateKey("ObserverUp", VK_NUMPAD8);
	CInput::GetInst()->CreateKey("ObserverDown", VK_NUMPAD2);
	CInput::GetInst()->CreateKey("ObserverLeft", VK_NUMPAD4);
	CInput::GetInst()->CreateKey("ObserverRight", VK_NUMPAD6);

	CInput::GetInst()->CreateKey("ObserverReset", VK_NUMPAD5);

	CInput::GetInst()->CreateKey("Up", 'W');
	CInput::GetInst()->CreateKey("Down", 'S');
	CInput::GetInst()->CreateKey("Left", 'A');
	CInput::GetInst()->CreateKey("Right", 'D');

	CInput::GetInst()->CreateKey("MoveFrameUp", 'W');
	CInput::GetInst()->SetCtrlkey("MoveFrameUp", true);

	CInput::GetInst()->CreateKey("MoveFrameDown", 'S');
	CInput::GetInst()->SetCtrlkey("MoveFrameDown", true);

	CInput::GetInst()->CreateKey("MoveFrameLeft", 'A');
	CInput::GetInst()->SetCtrlkey("MoveFrameLeft", true);

	CInput::GetInst()->CreateKey("MoveFrameRight", 'D');
	CInput::GetInst()->SetCtrlkey("MoveFrameRight", true);

	CInput::GetInst()->CreateKey("MoveFrameStartXLeft", 'J');
	CInput::GetInst()->CreateKey("MoveFrameStartXRight", 'L');
	CInput::GetInst()->CreateKey("MoveFrameStartYUp", 'I');
	CInput::GetInst()->CreateKey("MoveFrameStartYDown", 'K');

	CInput::GetInst()->CreateKey("MoveFrameEndXLeft", 'J');
	CInput::GetInst()->SetCtrlkey("MoveFrameEndXLeft", true);
	CInput::GetInst()->CreateKey("MoveFrameEndXRight", 'L');
	CInput::GetInst()->SetCtrlkey("MoveFrameEndXRight", true);
	CInput::GetInst()->CreateKey("MoveFrameEndYUp", 'I');
	CInput::GetInst()->SetCtrlkey("MoveFrameEndYUp", true);
	CInput::GetInst()->CreateKey("MoveFrameEndYDown", 'K');
	CInput::GetInst()->SetCtrlkey("MoveFrameEndYDown", true);


	// IMGUI로 에디터에서 사용할 윈도우를 만들어준다.
	m_SpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("SpriteWindow");
	m_DetailWindow = CIMGUIManager::GetInst()->AddWindow<CDetailWindow>("DetailWindow");
	m_EditorMenu = CIMGUIManager::GetInst()->AddWindow<CEditorMenu>("EditorMenu");
	m_ObjectHierarchy = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchy>("ObjectHierarchy");
	m_TileMapWindow = CIMGUIManager::GetInst()->AddWindow<CTileMapWindow>("TileMapWindow");
	

	CRenderManager::GetInst()->CreateLayer("DragLayer", 10);

	CInput::GetInst()->SetCallback("MouseLButton", KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetCallback("MouseLButton", KeyState_Push, this, &CEditorManager::MouseLButtonPush);
	CInput::GetInst()->SetCallback("MouseLButton", KeyState_Up, this, &CEditorManager::MouseLButtonUp);
	CInput::GetInst()->SetCallback("MouseRButton", KeyState_Push, this, &CEditorManager::MouseRButtonPush);

	CInput::GetInst()->SetCallback("Up", KeyState_Down, this, &CEditorManager::KeyboardUp);
	CInput::GetInst()->SetCallback("Down", KeyState_Down, this, &CEditorManager::KeyboardDown);
	CInput::GetInst()->SetCallback("Left", KeyState_Down, this, &CEditorManager::KeyboardLeft);
	CInput::GetInst()->SetCallback("Right", KeyState_Down, this, &CEditorManager::KeyboardRight);

	CInput::GetInst()->SetCallback("ObserverUp", KeyState_Push, this, &CEditorManager::CameraMoveUp);
	CInput::GetInst()->SetCallback("ObserverDown", KeyState_Push, this, &CEditorManager::CameraMoveDown);
	CInput::GetInst()->SetCallback("ObserverLeft", KeyState_Push, this, &CEditorManager::CameraMoveLeft);
	CInput::GetInst()->SetCallback("ObserverRight", KeyState_Push, this, &CEditorManager::CameraMoveRight);

	CInput::GetInst()->SetCallback("MoveFrameUp", KeyState_Down, this, &CEditorManager::MoveFrameUp);
	CInput::GetInst()->SetCallback("MoveFrameDown", KeyState_Down, this, &CEditorManager::MoveFrameDown);
	CInput::GetInst()->SetCallback("MoveFrameLeft", KeyState_Down, this, &CEditorManager::MoveFrameLeft);
	CInput::GetInst()->SetCallback("MoveFrameRight", KeyState_Down, this, &CEditorManager::MoveFrameRight);

	CInput::GetInst()->SetCallback("MoveFrameStartXLeft", KeyState_Down, this, &CEditorManager::MoveFrameStartXLeft);
	CInput::GetInst()->SetCallback("MoveFrameStartXRight", KeyState_Down, this, &CEditorManager::MoveFrameStartXRight);
	CInput::GetInst()->SetCallback("MoveFrameStartYUp", KeyState_Down, this, &CEditorManager::MoveFrameStartYUp);
	CInput::GetInst()->SetCallback("MoveFrameStartYDown", KeyState_Down, this, &CEditorManager::MoveFrameStartYDown);

	CInput::GetInst()->SetCallback("MoveFrameEndXLeft", KeyState_Down, this, &CEditorManager::MoveFrameEndXLeft);
	CInput::GetInst()->SetCallback("MoveFrameEndXRight", KeyState_Down, this, &CEditorManager::MoveFrameEndXRight);
	CInput::GetInst()->SetCallback("MoveFrameEndYUp", KeyState_Down, this, &CEditorManager::MoveFrameEndYUp);
	CInput::GetInst()->SetCallback("MoveFrameEndYDown", KeyState_Down, this, &CEditorManager::MoveFrameEndYDown);

	m_CameraMoveSpeed = 1000.0f;

	return true;
}

void CEditorManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CDefaultScene>();
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CEditorManager::MouseLButtonDown(float deltaTime)
{
	if (m_DragObj)
	{
		if (m_DragObj->IsPointInStart(CInput::GetInst()->GetMouseWorld2DPos()))
		{
			m_DragStart = true;
			m_EndPos = m_DragObj->GetEndPos();
		}
		else if (m_DragObj->IsPointInEnd(CInput::GetInst()->GetMouseWorld2DPos()))
		{
			m_DragEnd = true;
			m_StartPos = m_DragObj->GetStartPos();
		}
		else
			m_DragObj->SetStartPos(CInput::GetInst()->GetMouseWorld2DPos());
	}
}

void CEditorManager::MouseLButtonPush(float deltaTime)
{
	m_MouseLButtonPush = true;

	if (m_DragObj)
	{
		if (m_DragStart)
		{
			m_DragObj->SetStartPos(CInput::GetInst()->GetMouseWorld2DPos());
			m_DragObj->SetEndPos(m_EndPos);
		}
		else if (m_DragEnd)
		{
			m_DragObj->SetEndPos(CInput::GetInst()->GetMouseWorld2DPos());
			m_DragObj->SetStartPos(m_StartPos);
		}
		else
			m_DragObj->SetEndPos(CInput::GetInst()->GetMouseWorld2DPos());

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MouseLButtonUp(float deltaTime)
{
	m_MouseLButtonPush = false;

	if (m_DragObj)
	{
		m_SpriteWindow->CutSprite();

		m_DragStart = false;
		m_DragEnd = false;
	}
}

void CEditorManager::MouseRButtonPush(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 size = Vector2(m_DragObj->GetWorldScale().x * 0.5f, m_DragObj->GetWorldScale().y * 0.5f);

		m_DragObj->SetWorldPos(CInput::GetInst()->GetMouseWorld2DPos().x - size.x, CInput::GetInst()->GetMouseWorld2DPos().y - size.y, 0.0f);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::KeyboardUp(float deltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(0.0f, 1.0f, 0.0f);
		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::KeyboardDown(float deltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(0.0f, -1.0f, 0.0f);
		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::KeyboardLeft(float deltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(-1.0f, 0.0f, 0.0f);
		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::KeyboardRight(float deltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(1.0f, 0.0f, 0.0f);
		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::CameraMoveUp(float deltaTime)
{
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	camera->AddWorldPos(Vector3(0.0f, m_CameraMoveSpeed * deltaTime, 0.0f));
}

void CEditorManager::CameraMoveDown(float deltaTime)
{
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	camera->AddWorldPos(Vector3(0.0f, -m_CameraMoveSpeed * deltaTime, 0.0f));
}

void CEditorManager::CameraMoveLeft(float deltaTime)
{
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	camera->AddWorldPos(Vector3(-m_CameraMoveSpeed * deltaTime, 0.0f, 0.0f));
}

void CEditorManager::CameraMoveRight(float deltaTime)
{
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	camera->AddWorldPos(Vector3(m_CameraMoveSpeed * deltaTime, 0.0f, 0.0f));
}

void CEditorManager::MoveFrameUp(float deltaTime)
{
	if (m_DragObj)
	{
		float y = fabs(m_DragObj->GetWorldScale().y);

		m_DragObj->AddWorldPos(0.0f, y, 0.0f);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameDown(float deltaTime)
{
	if (m_DragObj)
	{
		float y = fabs(m_DragObj->GetWorldScale().y);

		m_DragObj->AddWorldPos(0.0f, -y, 0.0f);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameLeft(float detlaTime)
{
	if (m_DragObj)
	{
		float x = fabs(m_DragObj->GetWorldScale().x);

		m_DragObj->AddWorldPos(-x, 0.0f, 0.0f);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameRight(float deltaTime)
{
	if (m_DragObj)
	{
		float x = fabs(m_DragObj->GetWorldScale().x);

		m_DragObj->AddWorldPos(x, 0.0f, 0.0f);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameStartXRight(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragStart.x += 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameStartXLeft(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragStart.x -= 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameStartYUp(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragStart.y += 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameStartYDown(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragStart.y -= 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameEndXRight(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragEnd.x += 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameEndXLeft(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragEnd.x -= 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameEndYUp(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragEnd.y += 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);

		m_SpriteWindow->CutSprite();
	}
}

void CEditorManager::MoveFrameEndYDown(float deltaTime)
{
	if (m_DragObj)
	{
		Vector2 tempStart = Vector2(m_DragObj->GetWorldPos().x, m_DragObj->GetWorldPos().y);
		Vector2 tempEnd = Vector2(tempStart.x + m_DragObj->GetWorldScale().x, tempStart.y + m_DragObj->GetWorldScale().y);

		Vector2 dragStart;
		Vector2 dragEnd;

		dragStart.x = tempStart.x > tempEnd.x ? tempEnd.x : tempStart.x;
		dragStart.y = tempStart.y < tempEnd.y ? tempEnd.y : tempStart.y;

		dragEnd.x = tempStart.x > tempEnd.x ? tempStart.x : tempEnd.x;
		dragEnd.y = tempStart.y < tempEnd.y ? tempStart.y : tempEnd.y;

		dragEnd.y -= 1.0f;

		m_DragObj->SetStartPos(dragStart);
		m_DragObj->SetEndPos(dragEnd);


		m_SpriteWindow->CutSprite();
	}
}


void CEditorManager::CreateSceneMode(CScene* scene, size_t type)
{
	if (type == typeid(CDefaultScene).hash_code())
	{
		// 로드시에 씬 매니저에 묶인 이 함수가 씬 모드 생성시에 호출 된다.
		scene->LoadSceneMode<CDefaultScene>();
	}
}

CGameObject* CEditorManager::CreateObject(CScene* scene, size_t type)
{
	// 씬을 로드시에 씬 매니저에 오브젝트 생성시 묶인 이 함수가 호출된다.


	// 타입을 분별해서 해당 씬에 타입에 맞는 오브젝트를 로드한다.
	// 이때 초기화하는 하지 않고 로드해서 불러온 데이터로 초기화한다.
	// obj를 리턴하여 파일을 로드해서 정보 초기화
	if (type == typeid(CGameObject).hash_code())
	{
		CGameObject* obj = scene->LoadGameObject<CGameObject>();

		return obj;
	}
	else if (type == typeid(CDragObject).hash_code())
	{
		CDragObject* obj = scene->LoadGameObject<CDragObject>();

		return obj;
	}
	else if (type == typeid(CSpriteEditObject).hash_code())
	{
		CSpriteEditObject* obj = scene->LoadGameObject<CSpriteEditObject>();

		return obj;
	}
	else if (type == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* obj = scene->LoadGameObject<CPlayer2D>();

		return obj;
	}

	return nullptr;
}

CComponent* CEditorManager::CreateComponet(CGameObject* obj, size_t type)
{
	// 로드시 씬에서 컴포넌트를 생성시 씬 매니저에 묶인 이 함수가 호출된다.

	// 전달받은 오브젝트에 컴포넌트를 추가하고 그 컴포넌트를 반환한다.
	// 그 이후 컴포넌트의 로드 함수 호출
	if (type == typeid(CSceneComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSceneComponent>();

		return component;
	}
	else if (type == typeid(CSpriteComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSpriteComponent>();

		return component;
	}
	else if (type == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CStaticMeshComponent>();

		return component;
	}
	else if (type == typeid(CCameraComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CCameraComponent>();

		return component;
	}
	else if (type == typeid(CTileMapComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CTileMapComponent>();

		return component;
	}

	return nullptr;
}

void CEditorManager::CreateAnimInstance(CSpriteComponent* sprite, size_t type)
{
	if (type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		sprite->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}
}
