
#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "EffectHit.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../UI//WidgetComponent.h"
#include "../UI/NumberWidget.h"
#include "DamageFont.h"

CBullet::CBullet()
{
	m_ObjType = EObject_Type::Bullet;
	m_Dir.x = 1.f;
	m_Dir.y = 0.f;

	m_Distance = 800.f;

	SetMoveSpeed(250.0f);

	m_Damage = 20.0f;
}

CBullet::CBullet(const CBullet& obj)	:
	CGameObject(obj)
{
	m_Damage = obj.m_Damage;
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
}

CBullet::~CBullet()
{
}

void CBullet::Start()
{
	CGameObject::Start();

	CCollider* Body = FindCollider("Body");

	Body->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
}

bool CBullet::Init()
{
	if (!CGameObject::Init())
		return false;

	/*SetTexture("Bullet", TEXT("Hit2.bmp"));
	SetSize(178.0f, 164.0f);
	SetImageStart(178.0f * 5, 0.0f);
	SetTextureColorKey(255, 0, 255);*/

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("Bullet", true, 1.0f);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.0f);
	Body->SetOffset(0.0f, 0.0f);

	//CColliderBox* Body = AddCollider<CColliderBox>("Body");
	//Body->SetExtent(50.0f, 50.0f);
	//Body->SetOffset(0.0f, 0.0f);
	//Body->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2	Dir = m_Dir;
	Dir.Normalize();

	Move(Dir);

	m_Distance -= GetMoveSpeedFrame();

	if (m_Distance <= 0.f)
		Destroy();
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBullet::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	Destroy();

	Dest->GetOwner()->SetDamage(m_Damage);

	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit> ("HitEffect", 
		"HitEffect", m_Pos, Vector2(178.0f, 164.0f));

	CDamageFont* DamageFont =  m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);

	DamageFont->SetDamageNumber((int)m_Damage);
}
