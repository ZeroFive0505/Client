#include "Observer.h"
#include "Input.h"
#include "Engine.h"

CObserver::CObserver()
{
}

CObserver::CObserver(const CObserver& obj) :
	CGameObject(obj)
{
}

CObserver::~CObserver()
{
}

void CObserver::Start()
{
	CGameObject::Start();

	CInput::GetInst()->SetCallback<CObserver>("MoveUp", KeyState_Push, this, &CObserver::MoveUp);
	CInput::GetInst()->SetCallback<CObserver>("MoveDown", KeyState_Push, this, &CObserver::MoveDown);
	CInput::GetInst()->SetCallback<CObserver>("MoveLeft", KeyState_Push, this, &CObserver::MoveLeft);
	CInput::GetInst()->SetCallback<CObserver>("MoveRight", KeyState_Push, this, &CObserver::MoveRight);
	CInput::GetInst()->SetCallback<CObserver>("ResetPos", KeyState_Down, this, &CObserver::ResetPos);
}

bool CObserver::Init()
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

void CObserver::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CObserver::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CObserver* CObserver::Clone()
{
	return new CObserver(*this);
}

void CObserver::MoveUp(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_Y) * 1.0f);
}

void CObserver::MoveDown(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_Y) * -1.0f);
}

void CObserver::MoveLeft(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_X) * -1.0f);
}

void CObserver::MoveRight(float deltaTime)
{
	m_Transform->AddWorldPos(m_Transform->GetWorldAxis(AXIS_X) * 1.0f);
}

void CObserver::ResetPos(float deltaTime)
{
	sResolution rs = CEngine::GetInst()->GetResolution();

	m_Transform->SetWorldPos((float)rs.width / 2.0f, (float)rs.height / 2.0f, 0.0f);
}
