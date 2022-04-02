#include "EditorObserver.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Engine.h"

CEditorObserver::CEditorObserver()
{
}

CEditorObserver::CEditorObserver(const CEditorObserver& obj) :
	CGameObject(obj)
{
}

CEditorObserver::~CEditorObserver()
{
}

void CEditorObserver::Start()
{
	CGameObject::Start();

	CInput::GetInst()->SetCallback<CEditorObserver>("ObserverUp", KeyState_Push, this, &CEditorObserver::MoveUp);
	CInput::GetInst()->SetCallback<CEditorObserver>("ObserverDown", KeyState_Push, this, &CEditorObserver::MoveDown);
	CInput::GetInst()->SetCallback<CEditorObserver>("ObserverLeft", KeyState_Push, this, &CEditorObserver::MoveLeft);
	CInput::GetInst()->SetCallback<CEditorObserver>("ObserverRight", KeyState_Push, this, &CEditorObserver::MoveRight);
	CInput::GetInst()->SetCallback<CEditorObserver>("ObserverReset", KeyState_Down, this, &CEditorObserver::ResetPos);
}

bool CEditorObserver::Init()
{
	sResolution rs = CEngine::GetInst()->GetResolution();

	m_Transform = CreateComponent<CSceneComponent>("Trnasform");

	SetPivot(0.5f, 0.5f, 0.0f);
	SetWorldPos((float)rs.width / 2.0f, (float)rs.height / 2.0f, 0.0f);

	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_Transform->AddChild(m_Camera);

	m_Camera->OnViewPortCenter();

	return true;
}

void CEditorObserver::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CEditorObserver::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CEditorObserver* CEditorObserver::Clone()
{
	return new CEditorObserver(*this);
}

void CEditorObserver::Save(FILE* pFile)
{

}

void CEditorObserver::Load(FILE* pFile)
{

}

void CEditorObserver::MoveUp(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_Y) * 1.0f);
}

void CEditorObserver::MoveDown(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_Y) * -1.0f);
}

void CEditorObserver::MoveLeft(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_X) * -1.0f);
}

void CEditorObserver::MoveRight(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_X) * 1.0f);
}

void CEditorObserver::ResetPos(float deltaTime)
{
	sResolution rs = CEngine::GetInst()->GetResolution();
	m_Transform->SetWorldPos((float)rs.width / 2.0f, (float)rs.height / 2.0f, 0.0f);
}
