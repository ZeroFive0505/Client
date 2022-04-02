
#include "BulletCamera.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Enemy.h"

CBulletCamera::CBulletCamera() :
	m_Distance(600.0f)
{
	SetTypeID<CBulletCamera>();
	m_Damage = 1;
}

CBulletCamera::CBulletCamera(const CBulletCamera& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
	m_Body = (CColliderBox2D*)FindComponent("BulletBody");
	m_Camera = (CCameraComponent*)FindComponent("BulletCamera");
	m_Distance = obj.m_Distance;
}

CBulletCamera::~CBulletCamera()
{
}

void CBulletCamera::Start()
{
	CGameObject::Start();

	m_Scene->GetCameraManager()->SetPrevCamera();
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
}

bool CBulletCamera::Init()
{
	CGameObject::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");
	m_Body = CreateComponent<CColliderBox2D>("BulletBody");
	m_Camera = CreateComponent<CCameraComponent>("BulletCamera");

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Body);
	m_Sprite->AddChild(m_Camera);

	m_Camera->OnViewPortCenter();

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->AddCollisionCallback<CBulletCamera>(Collision_State::Begin, this, &CBulletCamera::CollisionCallback);

	m_Sprite->SetRelativeScale(50.0f, 50.0f, 1.0f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CBulletCamera::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	float dist = 500.0f * deltaTime;

	m_Distance -= dist;

	if (m_Distance <= 0.0f)
	{
		m_Scene->GetCameraManager()->ReturnToPrevCamera();
		Destroy();
	}

	AddRelativePos(GetWorldAxis(AXIS_X) * dist);
}

void CBulletCamera::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBulletCamera* CBulletCamera::Clone()
{
	return new CBulletCamera(*this);
}

void CBulletCamera::CollisionCallback(const sCollisionResult& result)
{
	m_Scene->GetCameraManager()->ReturnToPrevCamera();

	if (result.dest->GetCollisionProfile()->channel == Collision_Channel::Enemy)
	{
		((CEnemy*)result.dest->GetGameObject())->DealDamage(m_Damage);
	}

	Destroy();
}
