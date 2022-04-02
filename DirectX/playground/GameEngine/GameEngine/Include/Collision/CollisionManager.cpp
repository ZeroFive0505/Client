#include "CollisionManager.h"


DEFINITION_SINGLE(CCollisionManager)

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
}

bool CCollisionManager::Init()
{
	CreateProfile("Object", Collision_Channel::Object, true);
	CreateProfile("Player", Collision_Channel::Player, true);
	CreateProfile("PlayerBottom", Collision_Channel::PlayerBottom, true);
	CreateProfile("Enemy", Collision_Channel::Enemy, true);
	CreateProfile("PlayerContactBox", Collision_Channel::PlayerContactBox, true);
	CreateProfile("EnemyContactBox", Collision_Channel::EnemyContactBox, true);
	CreateProfile("EnemyBottom", Collision_Channel::EnemyBottom, true);
	CreateProfile("PlayerAttack", Collision_Channel::PlayerAttack, true);
	CreateProfile("EnemyAttack", Collision_Channel::EnemyAttack, true);
	CreateProfile("Wall", Collision_Channel::Wall, true);
	CreateProfile("SideWall", Collision_Channel::SideWall, true);

	SetCollisionState("Player", Collision_Channel::Player, Collision_Interaction::Ignore);
	SetCollisionState("Player", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("Player", Collision_Channel::PlayerBottom, Collision_Interaction::Collision);
	SetCollisionState("Player", Collision_Channel::Enemy, Collision_Interaction::Collision);
	SetCollisionState("PlayerContactBox", Collision_Channel::Player, Collision_Interaction::Ignore);
	SetCollisionState("PlayerContactBox", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("PlayerContactBox", Collision_Channel::PlayerBottom, Collision_Interaction::Ignore);
	SetCollisionState("PlayerContactBox", Collision_Channel::EnemyBottom, Collision_Interaction::Ignore);
	SetCollisionState("PlayerContactBox", Collision_Channel::Enemy, Collision_Interaction::Ignore);
	SetCollisionState("PlayerContactBox", Collision_Channel::EnemyContactBox, Collision_Interaction::Collision);

	SetCollisionState("Enemy", Collision_Channel::Enemy, Collision_Interaction::Ignore);
	SetCollisionState("Enemy", Collision_Channel::EnemyAttack, Collision_Interaction::Ignore);
	SetCollisionState("Enemy", Collision_Channel::EnemyBottom, Collision_Interaction::Collision);
	SetCollisionState("Enemy", Collision_Channel::Player, Collision_Interaction::Collision);
	SetCollisionState("EnemyContactBox", Collision_Channel::Enemy, Collision_Interaction::Ignore);
	SetCollisionState("EnemyContactBox", Collision_Channel::EnemyAttack, Collision_Interaction::Ignore);
	SetCollisionState("EnemyContactBox", Collision_Channel::EnemyBottom, Collision_Interaction::Ignore);
	SetCollisionState("EnemyContactBox", Collision_Channel::PlayerBottom, Collision_Interaction::Ignore);
	SetCollisionState("EnemyContactBox", Collision_Channel::Player, Collision_Interaction::Ignore);
	SetCollisionState("EnemyContactBox", Collision_Channel::PlayerContactBox, Collision_Interaction::Collision);

	SetCollisionState("PlayerAttack", Collision_Channel::Player, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::PlayerBottom, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::EnemyAttack, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::PlayerContactBox, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::EnemyContactBox, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::EnemyBottom, Collision_Interaction::Ignore);

	SetCollisionState("EnemyAttack", Collision_Channel::Enemy, Collision_Interaction::Ignore);
	SetCollisionState("EnmeyAttack", Collision_Channel::EnemyBottom, Collision_Interaction::Ignore);
	SetCollisionState("EnemyAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("EnemyAttack", Collision_Channel::EnemyAttack, Collision_Interaction::Ignore);
	SetCollisionState("EnemyAttack", Collision_Channel::EnemyContactBox, Collision_Interaction::Ignore);
	SetCollisionState("EnemyAttack", Collision_Channel::PlayerContactBox, Collision_Interaction::Ignore);
	SetCollisionState("EnemyAttack", Collision_Channel::PlayerBottom, Collision_Interaction::Ignore);

	IgnoreAllButThis("Wall", Collision_Channel::PlayerBottom);
	SetCollisionState("Wall", Collision_Channel::EnemyBottom, Collision_Interaction::Collision);
	IgnoreAllButThis("SideWall", Collision_Channel::PlayerContactBox);

	SetCollisionState("SideWall", Collision_Channel::EnemyContactBox, Collision_Interaction::Collision);

	SetCollisionState("PlayerBottom", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("EnemyBottom", Collision_Channel::EnemyAttack, Collision_Interaction::Ignore);

	IgnoreAllButThis("Object", Collision_Channel::PlayerBottom);
	SetCollisionState("PlayerBottom", Collision_Channel::Object, Collision_Interaction::Collision);

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& name, Collision_Channel channel, bool enable, Collision_Interaction interaction)
{
	sCollisionProfile* profile = FindProfile(name);

	if (profile)
		return false;

	profile = new sCollisionProfile;

	profile->name = name;
	profile->channel = channel;
	profile->collisionEnable = enable;
	// 모든 채널과의 관계를 저장
	profile->vecInteraction.resize((int)Collision_Channel::MAX);


	// 해당 프로파일과 다른 채널과의 충돌을 설정
	for (int i = 0; i < (int)Collision_Channel::MAX; i++)
	{
		profile->vecInteraction[i] = interaction;
	}


	m_mapProfile.insert(std::make_pair(name, profile));

	return true;
}

bool CCollisionManager::SetCollisionState(const std::string& name, Collision_Channel channel, Collision_Interaction interaction)
{
	sCollisionProfile* profile = FindProfile(name);

	if (!profile)
		return false;

	profile->vecInteraction[(int)channel] = interaction;

	return true;
}

sCollisionProfile* CCollisionManager::FindProfile(const std::string& name)
{
	auto iter = m_mapProfile.find(name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
