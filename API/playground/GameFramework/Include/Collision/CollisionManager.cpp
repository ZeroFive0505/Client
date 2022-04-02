#include "CollisionManager.h"

CCollisionManager* CCollisionManager::m_Inst = nullptr;

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	auto iter = m_mapProfile.begin();
	auto iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapProfile.clear();
}

// �⺻ �������ϵ��� �����Ѵ�.
bool CCollisionManager::Init()
{
	// �浹 �������ϵ��� �����Ѵ�.
	CreateProfile("Default", Channel_Object, true);
	CreateProfile("Player", Channel_Player, true);
	CreateProfile("Monster", Channel_Monster, true);
	CreateProfile("PlayerAttack", Channel_PlayerAttack, true);
	CreateProfile("MonsterAttack", Channel_MonsterAttack, true);

	// ���������� ���μ����� �����Ѵ�.
	SetCollisionState("Player", Channel_Player, ECollision_State::Ignore);
	SetCollisionState("Player", Channel_PlayerAttack, ECollision_State::Ignore);

	SetCollisionState("Monster", Channel_Monster, ECollision_State::Ignore);
	SetCollisionState("Monster", Channel_MonsterAttack, ECollision_State::Ignore);

	SetCollisionState("PlayerAttack", Channel_Player, ECollision_State::Ignore);
	SetCollisionState("PlayerAttack", Channel_PlayerAttack, ECollision_State::Ignore);
	SetCollisionState("PlayerAttack", Channel_MonsterAttack, ECollision_State::Ignore);

	SetCollisionState("MonsterAttack", Channel_Monster, ECollision_State::Ignore);
	SetCollisionState("MonsterAttack", Channel_PlayerAttack, ECollision_State::Ignore);
	SetCollisionState("MonsterAttack", Channel_MonsterAttack, ECollision_State::Ignore);

	return true;
}

// ���������� ���Ӱ� �����Ѵ�.
bool CCollisionManager::CreateProfile(const std::string& Name, ECollision_Channel Channel, bool Enable, ECollision_State State)
{
	// ���� �̹� ���� ���������� �����Ѵٸ�
	CollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	Profile = new CollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->CollisionEnable = Enable;
	Profile->vecState.resize(Channel_Max);

	for (int i = 0; i < Channel_Max; i++)
	{
		Profile->vecState[i] = State;
	}

	m_mapProfile.insert(std::make_pair(Name, Profile));

	return true;
}

// �������ϰ� �浹ü�� ä���� ������ �����Ѵ�.
bool CCollisionManager::SetCollisionState(const std::string& Name, ECollision_Channel Channel, ECollision_State State)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	Profile->vecState[Channel] = State;

	return true;
}

// ���������� ã�ƿ´�.
CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	auto iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}