#include "Pendulum.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Input.h"

CPendulum::CPendulum() :
	m_Line(nullptr)
{
	SetTypeID<CPendulum>();
}

CPendulum::CPendulum(const CPendulum& obj)
{
}

CPendulum::~CPendulum()
{
}

bool CPendulum::Init()
{
	sResolution rs = CEngine::GetInst()->GetResolution();

	m_Origin = Vector2((float)rs.width / 2.0f, (float)rs.height);

	m_ArmLength = 500.0f;
	m_Angle = PI / 4;
	m_AngularVelocity = 0.0f;
	m_AngularAcceleration = 0.0f;

	m_Gravity = -0.1f;

	m_Damping = 1.0f;

	m_Line = m_Scene->CreateGameObject<CLine>("Line");
	m_Line->SetWorldPos(m_Origin.x, m_Origin.y, 0.0f);
	m_Line->SetWorldScale(m_ArmLength, 1.0f, 1.0f);

	m_Sprite = CreateComponent<CSpriteComponent>("Sprite");
	// m_Body = CreateComponent<CColliderCircle>("Circle");

	SetRootComponent(m_Sprite);

	// m_Sprite->AddChild(m_Body);

	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_Sprite->SetWorldScale(50.0f, 50.0f, 1.0f);

	// m_Body->SetInfo(Vector2(0.0f, 0.0f), 25.0f);

	// m_Body->AddCollisionMouseCallback<CPendulum>(Collision_State::Begin, this, &CPendulum::OnMouseCollision);
	// m_Body->AddCollisionMouseCallback<CPendulum>(Collision_State::End, this, &CPendulum::OnMouseCollisionExit);

	m_Line->SetWorldRotationZ(RadianToDegree(m_Angle));

	m_Sprite->SetWorldPos(m_Origin.x + m_ArmLength * cosf(m_Angle), m_Origin.y + m_ArmLength * sinf(m_Angle), 0.0f);

	return true;
}

void CPendulum::Start()
{
	CGameObject::Start();
}

void CPendulum::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	
	if(m_OnMouse && CInput::GetInst()->GetMouseLButtonClick())
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

		Vector2 originPos = Vector2(m_Origin.x, m_Origin.y);

		Vector2 dir = mousePos - originPos;

		m_Angle = atan2f(dir.y, dir.x);
		m_Angle = m_Angle + PI / 2;
		m_Sprite->SetWorldPos(m_Origin.x + m_ArmLength * cosf(m_Angle - PI / 2), m_Origin.y + m_ArmLength * sin(m_Angle - PI / 2), 0.0f);
		m_Line->SetWorldRotationZ(RadianToDegree(m_Angle - PI / 2));
	}
	else
	{
		m_AngularAcceleration = (m_Gravity / m_ArmLength) * sinf(m_Angle);

		m_AngularVelocity += m_AngularAcceleration;

		m_Angle += m_AngularVelocity;

		m_AngularVelocity *= m_Damping;

		m_Sprite->SetWorldPos(m_Origin.x + m_ArmLength * cosf(m_Angle - PI / 2), m_Origin.y + m_ArmLength * sin(m_Angle - PI / 2), 0.0f);
		m_Line->SetWorldRotationZ(RadianToDegree(m_Angle - PI / 2));
	}
}

void CPendulum::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CPendulum* CPendulum::Clone()
{
	return new CPendulum(*this);
}
