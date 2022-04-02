#include "Liquid.h"
#include "Engine.h"
#include "Mover.h"

CLiquid::CLiquid()
{
	SetTypeID<CLiquid>();
}

CLiquid::CLiquid(const CLiquid& obj)
{
	m_Transform = (CSceneComponent*)FindComponent("LiquidTransform");
	m_Collider = (CColliderBox2D*)FindComponent("LiquidCollider");
}

CLiquid::~CLiquid()
{
}

void CLiquid::ApplyDrag(float deltaTime)
{
	size_t size = m_vecMovers.size();

	for (size_t i = 0; i < size; i++)
	{
		Vector2 vel = m_vecMovers[i]->GetVelocity();
		Vector2 dir = vel;

		float speed = vel.Length();

		dir.Normalize();

		Vector2 drag = dir;

		drag *= -m_DragConstant * speed * speed;

		m_vecMovers[i]->ApplyForce(drag * deltaTime);
	}
}

void CLiquid::AddMover(const sCollisionResult& result)
{
	m_vecMovers.push_back((CMover*)(result.dest->GetGameObject()));
}

bool CLiquid::Init()
{
	m_Transform = CreateComponent<CSceneComponent>("LiquidTransform");

	m_Transform->SetPivot(0.5f, 0.5f, 0.0f);

	sResolution rs = CEngine::GetInst()->GetResolution();

	m_Transform->SetWorldPos((float)rs.width / 2.0f, 0.0f, 0.0f);

	m_Collider = CreateComponent<CColliderBox2D>("LiquidCollider");

	m_Collider->AddCollisionCallback<CLiquid>(Collision_State::Begin, this, &CLiquid::AddMover);
	
	m_Transform->AddChild(m_Collider);

	m_Collider->SetExtent((float)rs.width, 350.0f);

	m_DragConstant = 220.0f;

	return true;
}

void CLiquid::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	ApplyDrag(deltaTime);
}

void CLiquid::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CLiquid* CLiquid::Clone()
{
	return new CLiquid(*this);
}
