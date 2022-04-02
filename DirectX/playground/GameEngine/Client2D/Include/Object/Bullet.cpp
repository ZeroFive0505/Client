#include "Bullet.h"
#include "Component/SpriteComponent.h"

CBullet::CBullet() :
	m_Distance(600.0f)
{
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(50.0f, 50.0f, 1.0f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CBullet::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	float dist = 500.0f * deltaTime;

	m_Distance -= dist;

	if (m_Distance <= 0.0f)
		Destroy();

	AddRelativePos(GetWorldAxis(AXIS_Y) * dist);
}

void CBullet::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}
