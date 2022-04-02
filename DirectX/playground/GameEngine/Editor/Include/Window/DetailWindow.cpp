#include "DetailWindow.h"
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
#include "Component/SpriteComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "../Object/DragObject.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "IMGUIManager.h"
#include "ObjectHierarchy.h"

CDetailWindow::CDetailWindow() :
	m_PosX(nullptr),
	m_PosY(nullptr),
	m_PosZ(nullptr),
	m_RotX(nullptr),
	m_RotY(nullptr),
	m_RotZ(nullptr),
	m_ScaleX(nullptr),
	m_ScaleY(nullptr),
	m_ScaleZ(nullptr)
{
}

CDetailWindow::~CDetailWindow()
{
}

bool CDetailWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* label = AddWidget<CIMGUILabel>("Transform", 100.0f, 30.0f);

	label->SetColor(112, 128, 144);
	label->SetAlign(0.5f, 0.0f);

	label = AddWidget<CIMGUILabel>("Position", 60.0f, 30.0f);

	label->SetColor(47, 79, 79);
	label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("X", 30.0f, 30.0f);

	label->SetColor(255, 0, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_PosX = AddWidget<CIMGUITextInput>("PositionX", 60.0f, 30.0f);
	m_PosX->SetHideName(true);
	m_PosX->SetTextType(ImGuiText_Type::Float);
	m_PosX->SetCallback(this, &CDetailWindow::PositionXCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("Y", 30.0f, 30.0f);

	label->SetColor(0, 255, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_PosY = AddWidget<CIMGUITextInput>("PositionY", 60.0f, 30.0f);
	m_PosY->SetHideName(true);
	m_PosY->SetTextType(ImGuiText_Type::Float);
	m_PosY->SetCallback(this, &CDetailWindow::PositionYCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("Z", 30.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_PosZ = AddWidget<CIMGUITextInput>("PositionZ", 60.0f, 30.0f);
	m_PosZ->SetHideName(true);
	m_PosZ->SetTextType(ImGuiText_Type::Float);
	m_PosZ->SetCallback(this, &CDetailWindow::PositionZCallbacK);

	label = AddWidget<CIMGUILabel>("Rotation", 60.0f, 30.0f);
	label->SetColor(47, 79, 79);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("X", 30.0f, 30.0f);
	label->SetColor(255, 0, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_RotX = AddWidget<CIMGUITextInput>("RotationX", 60.0f, 30.0f);
	m_RotX->SetHideName(true);
	m_RotX->SetTextType(ImGuiText_Type::Float);
	m_RotX->SetCallback(this, &CDetailWindow::RotationXCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("Y", 30.0f, 30.0f);
	label->SetColor(0, 255, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_RotY = AddWidget<CIMGUITextInput>("RotationY", 60.0f, 30.0f);
	m_RotY->SetHideName(true);
	m_RotY->SetTextType(ImGuiText_Type::Float);
	m_RotY->SetCallback(this, &CDetailWindow::RotationYCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("Z", 30.0f, 30.0f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);
	
	line = AddWidget<CIMGUISameLine>("Line");

	m_RotZ = AddWidget<CIMGUITextInput>("RotationZ", 60.0f, 30.0f);
	m_RotZ->SetHideName(true);
	m_RotZ->SetTextType(ImGuiText_Type::Float);
	m_RotZ->SetCallback(this, &CDetailWindow::RotationZCallback);

	label = AddWidget<CIMGUILabel>("Scale", 60.0f, 30.0f);
	label->SetColor(47, 79, 79);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("X", 30.0f, 30.0f);
	label->SetColor(255, 0, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleX = AddWidget<CIMGUITextInput>("ScaleX", 60.0f, 30.0f);
	m_ScaleX->SetHideName(true);
	m_ScaleX->SetTextType(ImGuiText_Type::Float);
	m_ScaleX->SetFloat(1.0f);
	m_ScaleX->SetCallback(this, &CDetailWindow::ScaleXCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("Y", 30.0f, 30.0f);
	label->SetColor(0, 255, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleY = AddWidget<CIMGUITextInput>("ScaleY", 60.0f, 30.0f);
	m_ScaleY->SetHideName(true);
	m_ScaleY->SetTextType(ImGuiText_Type::Float);
	m_ScaleY->SetFloat(1.0f);
	m_ScaleY->SetCallback(this, &CDetailWindow::ScaleYCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("Z", 30.0f, 30.0f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleZ = AddWidget<CIMGUITextInput>("ScaleZ", 60.0f, 30.0f);
	m_ScaleZ->SetHideName(true);
	m_ScaleZ->SetTextType(ImGuiText_Type::Float);
	m_ScaleZ->SetFloat(1.0f);
	m_ScaleZ->SetCallback(this, &CDetailWindow::ScaleZCallback);

	return true;
}

void CDetailWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CDetailWindow::PositionXCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	// 현재 오브젝트 리스트에서 선택된 아이템이 없다면 반환
	if (objList->GetSelectIndex() == -1)
		return;

	// 이름으로 가져온다.
	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 pos = obj->GetRootComponent()->GetWorldPos();

	pos.x = m_PosX->GetValueFloat();

	obj->GetRootComponent()->SetWorldPos(pos);
}

void CDetailWindow::PositionYCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	// 현재 오브젝트 리스트에서 선택된 아이템이 없다면 반환
	if (objList->GetSelectIndex() == -1)
		return;

	// 이름으로 가져온다.
	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 pos = obj->GetRootComponent()->GetWorldPos();

	pos.y = m_PosY->GetValueFloat();

	obj->GetRootComponent()->SetWorldPos(pos);
}

void CDetailWindow::PositionZCallbacK()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	// 현재 오브젝트 리스트에서 선택된 아이템이 없다면 반환
	if (objList->GetSelectIndex() == -1)
		return;

	// 이름으로 가져온다.
	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 pos = obj->GetRootComponent()->GetWorldPos();

	pos.z = m_PosZ->GetValueFloat();

	obj->GetRootComponent()->SetWorldPos(pos);
}

void CDetailWindow::RotationXCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	if (objList->GetSelectIndex() == -1)
		return;

	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 rot = obj->GetRootComponent()->GetWorldRot();

	rot.x = m_RotX->GetValueFloat();

	obj->GetRootComponent()->SetWorldRotation(rot);
}

void CDetailWindow::RotationYCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	if (objList->GetSelectIndex() == -1)
		return;

	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 rot = obj->GetRootComponent()->GetWorldRot();

	rot.y = m_RotY->GetValueFloat();

	obj->GetRootComponent()->SetWorldRotation(rot);
}

void CDetailWindow::RotationZCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	if (objList->GetSelectIndex() == -1)
		return;

	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 rot = obj->GetRootComponent()->GetWorldRot();

	rot.z = m_RotZ->GetValueFloat();
	
	obj->GetRootComponent()->SetWorldRotation(rot);
}

void CDetailWindow::ScaleXCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	if (objList->GetSelectIndex() == -1)
		return;

	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 scale = obj->GetRootComponent()->GetWorldScale();

	scale.x = m_ScaleX->GetValueFloat();

	obj->GetRootComponent()->SetWorldScale(scale);
}

void CDetailWindow::ScaleYCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	if (objList->GetSelectIndex() == -1)
		return;

	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 scale = obj->GetRootComponent()->GetWorldScale();

	scale.y = m_ScaleY->GetValueFloat();

	obj->GetRootComponent()->SetWorldScale(scale);
}

void CDetailWindow::ScaleZCallback()
{
	CObjectHierarchy* hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!hierarchy)
		return;

	CGameObject* obj = nullptr;

	CIMGUIListBox* objList = hierarchy->GetObjectList();

	if (objList->GetSelectIndex() == -1)
		return;

	obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

	if (!obj)
		return;

	Vector3 scale = obj->GetRootComponent()->GetWorldScale();

	scale.z = m_ScaleY->GetValueFloat();

	obj->GetRootComponent()->SetWorldScale(scale);
}
