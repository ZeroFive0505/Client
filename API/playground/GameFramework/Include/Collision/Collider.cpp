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
		// �� �浹ü�� ������� �� �浹ü�� �ٸ� �浹ü�� �浹����Ʈ�� �� �� �����Ƿ� �������ش�.
		(*iter)->DeleteCollisionList(this);
	}
}

// �浹ü ���������� �����Ѵ�.
void CCollider::SetCollisionProfile(const std::string& Name)
{
	// CollisionManager���� �ش� �浹ü�� ���������� �����´�.
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CCollider::AddCollisionList(CCollider* Collider)
{
	// �浹ü ����Ʈ�� �߰��Ѵ�.
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
		// �浹ü����Ʈ�� ���� �浹ü�� ã�Ƽ� �����Ѵ�.
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

// �ʱ�ȭ�ÿ��� Default �������Ϸ� �����̵ȴ�.
bool CCollider::Init()
{
	// �ʱⰪ
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
