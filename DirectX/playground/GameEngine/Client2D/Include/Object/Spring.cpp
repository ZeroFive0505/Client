#include "Spring.h"
#include "Scene/Scene.h"
#include "Input.h"


CSpring::CSpring() :
	m_Mover(nullptr),
	m_Spring(nullptr)
{
	SetTypeID<CSpring>();
}

CSpring::CSpring(const CSpring& obj)
{
	m_Spring = (CStaticMeshComponent*)FindComponent("Spring");
}

CSpring::~CSpring()
{
}

bool CSpring::Init()
{
	m_Spring = CreateComponent<CStaticMeshComponent>("Spring");

	SetRootComponent(m_Spring);

	m_Spring->SetMesh("LineMesh");
	m_Spring->GetMaterial()->SetShader("LineShader");
	m_Spring->SetBaseColor(0.0f, 1.0f, 0.0f, 1.0f);

	m_AngularVelocity = 0.0f;

	m_Damping = 0.95f;

	return true;
}

void CSpring::Start()
{
	CGameObject::Start();
}

void CSpring::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	if (m_Mover)
	{
		Vector2 moverPos = Vector2(m_Mover->GetWorldPos().x, m_Mover->GetWorldPos().y);
		Vector2 dir = moverPos - m_Anchor;

		float length = dir.Length();

		m_Spring->SetWorldScale(length, 1.0f, 1.0f);
		float angle = atan2f(dir.y, dir.x);
		angle = angle + PI / 2;
		m_Spring->SetWorldRotationZ(RadianToDegree(angle - PI / 2));

		if (!m_Mover->IsOnMouse())
		{
			float stretch = m_RestLength - length;

			dir.Normalize();

			Vector2 force = dir * -1.0f * m_SpringConstant;

			m_Mover->ApplyForce(force);

		}
	}
		
	ConstrainLength();
}

void CSpring::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CSpring* CSpring::Clone()
{
	return new CSpring(*this);
}

void CSpring::ConstrainLength()
{
	if (m_Mover && !m_Mover->IsOnMouse())
	{
		Vector2 moverPos = Vector2(m_Mover->GetWorldPos().x, m_Mover->GetWorldPos().y);
		Vector2 dir = moverPos - m_Anchor;

		float dist = dir.Length();

		float minLength = 50.0f;
		float maxLength = m_RestLength * 1.5f;

		if (dist < minLength)
		{
			dir.Normalize();
			dir *= -minLength;
			m_Mover->SetWorldPos(m_Anchor.x - dir.x, m_Anchor.y - dir.y, 0.0f);
			m_Mover->SetVelocity(Vector2(0.0f, 0.0f));
		}
		else if (dist > maxLength)
		{
			dir.Normalize();
			dir *= -maxLength;
			m_Mover->SetWorldPos(m_Anchor.x + dir.x, m_Anchor.y + dir.y, 0.0f);
			m_Mover->SetVelocity(Vector2(0.0f, 0.0f));
		}
	}
}
