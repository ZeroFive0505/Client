#include "Collider.h"
#include "CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"

CCollider::CCollider() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Enable(true),
	m_Profile(nullptr),
	m_MouseCollision(false)
{
}

CCollider::CCollider(const CCollider& collider) :
	CRef(collider),
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Enable(true)
{
	m_Type = collider.m_Type;
	m_Offset = collider.m_Offset;
	m_Profile = collider.m_Profile;
}

CCollider::~CCollider()
{
	m_Scene->GetSceneCollision()->ClearMouseCollision(this);

	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		// 이 충돌체가 사라질때 이 충돌체가 다른 충돌체의 충돌리스트에 들어갈 수 있으므로 제거해준다.
		(*iter)->DeleteCollisionList(this);
	}
}

// 충돌체 프로파일을 설정한다.
void CCollider::SetCollisionProfile(const std::string& Name)
{
	// CollisionManager에서 해당 충돌체의 프로파일을 가져온다.
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CCollider::AddCollisionList(CCollider* Collider)
{
	// 충돌체 리스트에 추가한다.
	m_CollisionList.push_back(Collider);
}

bool CCollider::CheckCollisionList(CCollider* Collider)
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

void CCollider::DeleteCollisionList(CCollider* Collider)
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; iter++)
	{	
		// 충돌체리스트에 들어온 충돌체를 찾아서 제거한다.
		if (*iter == Collider)
		{
			m_CollisionList.erase(iter);
			return;
		}
	}
}

void CCollider::ClearCollisionList()
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->DeleteCollisionList(this);
	}

	m_CollisionList.clear();
}

void CCollider::CallCollisionBegin(CCollider* Dest, float DeltaTime)
{
	if (m_BeginFunction)
		m_BeginFunction(this, Dest, DeltaTime);
}

void CCollider::CallCollisionEnd(CCollider* Dest, float DeltaTime)
{
	if (m_EndFunction)
		m_EndFunction(this, Dest, DeltaTime);
}

void CCollider::CallMouseCollsionBegin(const Vector2& MousePos, float DeltaTime)
{
	if (m_MouseBeginFunction)
		m_MouseBeginFunction(this, MousePos, DeltaTime);
}

void CCollider::CallMouseCollsionEnd(const Vector2& MousePos, float DeltaTime)
{
	if (m_MouseEndFunction)
		m_MouseEndFunction(this, MousePos, DeltaTime);
}

// 초기화시에는 Default 프로파일로 생성이된다.
bool CCollider::Init()
{
	// 초기값
	m_Profile = CCollisionManager::GetInst()->FindProfile("Default");

	return true;
}

void CCollider::Update(float DeltaTime)
{
}

void CCollider::PostUpdate(float DeltaTime)
{
}

void CCollider::Render(HDC hDC)
{
}

CCollider* CCollider::Clone()
{
	return nullptr;
}
