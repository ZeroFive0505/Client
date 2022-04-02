#include "Vehicle.h"
#include "Engine.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "FlowField.h"

CVehicle::CVehicle()
{
	SetTypeID<CVehicle>();
}

CVehicle::CVehicle(const CVehicle& obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Vehicle");
}

CVehicle::~CVehicle()
{
}

bool CVehicle::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("Vehicle");

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Car", TEXT("car.png"));

	m_Sprite->SetWorldScale(118.0f, 49.0f, 1.0f);

	sResolution rs = CEngine::GetInst()->GetResolution();

	SetWorldPos((float)rs.width / 2.0f, (float)rs.height / 2.0f, 0.0f);

	SetPivot(0.5f, 0.5f, 0.0f);

	m_Resolution.x = (float)rs.width;
	m_Resolution.y = (float)rs.height;

	m_Mass = 1.0f;

	m_MaxSpeed = 1.0f;

	m_MaxForce = 0.01f;

	return true;
}

void CVehicle::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	Follow();
	// Arrive(CInput::GetInst()->GetMouseWorld2DPos(), deltaTime);
	// Seek(CInput::GetInst()->GetMouseWorld2DPos(), deltaTime);
	// Flee(CInput::GetInst()->GetMouseWorld2DPos(), deltaTime);

	CheckEdge();
}

void CVehicle::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CVehicle* CVehicle::Clone()
{
	return new CVehicle(*this);
}

void CVehicle::Seek(const Vector2& target, float deltaTime)
{
	Vector2 currentLoc = Vector2(m_Sprite->GetWorldPos().x, m_Sprite->GetWorldPos().y);
	Vector2 desired = target - currentLoc;
	desired.Normalize();
	desired *= m_MaxSpeed;

	Vector2 steer = desired - m_Velocity;

	float angle = atan2f(desired.y, desired.x);

	m_Sprite->SetWorldRotationZ(RadianToDegree(angle));

	steer.x = std::fmaxf(-m_MaxForce, std::fminf(steer.x, m_MaxForce));
	steer.y = std::fmaxf(-m_MaxForce, std::fminf(steer.y, m_MaxForce));

	ApplyForce(steer * deltaTime);
}

void CVehicle::Flee(const Vector2& target, float deltaTime)
{
	Vector2 currentLoc = Vector2(m_Sprite->GetWorldPos().x, m_Sprite->GetWorldPos().y);
	Vector2 desired = target - currentLoc;
	desired *= -1.0f;
	desired.Normalize();
	desired *= m_MaxSpeed;

	Vector2 steer = desired - m_Velocity;

	float angle = atan2f(desired.y, desired.x);

	m_Sprite->SetWorldRotationZ(RadianToDegree(angle));

	steer.x = std::fmaxf(-m_MaxForce, std::fminf(steer.x, m_MaxForce));
	steer.y = std::fmaxf(-m_MaxForce, std::fminf(steer.y, m_MaxForce));

	ApplyForce(steer * deltaTime);
}

void CVehicle::Arrive(const Vector2& target, float deltaTime)
{
	Vector2 currentLoc = Vector2(m_Sprite->GetWorldPos().x, m_Sprite->GetWorldPos().y);
	Vector2 desired = target - currentLoc;
	float angle = atan2f(desired.y, desired.x);

	m_Sprite->SetWorldRotationZ(RadianToDegree(angle));

	float dist = desired.Length();

	desired.Normalize();

	if (dist < 50.0f)
	{
		float m = MapValue(dist, 0.0f, 100.0f, 0.0f, m_MaxSpeed);
		desired *= m;
	}
	else
		desired *= m_MaxSpeed;

	Vector2 steer = desired - m_Velocity;

	steer.x = std::fmaxf(-m_MaxForce, std::fminf(steer.x, m_MaxForce));
	steer.y = std::fmaxf(-m_MaxForce, std::fminf(steer.y, m_MaxForce));

	ApplyForce(steer);
}

void CVehicle::Follow()
{
	Vector2 desired = m_Field->LookUp(Vector2(m_Sprite->GetWorldPos().x, m_Sprite->GetWorldPos().y));
	desired *= m_MaxSpeed;

	Vector2 steer = desired - m_Velocity;

	steer.x = std::fmaxf(-m_MaxForce, std::fminf(steer.x, m_MaxForce));
	steer.y = std::fmaxf(-m_MaxForce, std::fminf(steer.y, m_MaxForce));

	float angle = atan2f(desired.y, desired.x);

	m_Sprite->SetWorldRotationZ(RadianToDegree(angle));

	ApplyForce(steer);
}

void CVehicle::ApplyForce(const Vector2& force)
{
	Vector2 accel = force / m_Mass;
	m_Acceleration = accel;
	m_Velocity += m_Acceleration;

	AddWorldPos(m_Velocity.x, 0.0f, 0.0f);
	AddWorldPos(0.0f, m_Velocity.y, 0.0f);

	m_Acceleration *= 0.0f;
}

void CVehicle::ApplyForce(float x, float y)
{
	Vector2 accel = Vector2(x, y);
	accel /= m_Mass;
	m_Velocity += m_Acceleration;

	AddWorldPos(m_Velocity.x, 0.0f, 0.0f);
	AddWorldPos(0.0f, m_Velocity.y, 0.0f);

	m_Acceleration *= 0.0f;
}

void CVehicle::CheckEdge()
{
	if (m_Sprite->GetWorldPos().x < 0.0f)
		m_Sprite->SetWorldPos(m_Resolution.x - 1.0f, m_Sprite->GetWorldPos().y, 0.0f);
	
	if (m_Sprite->GetWorldPos().x >= m_Resolution.x)
		m_Sprite->SetWorldPos(0.0f, m_Sprite->GetWorldPos().y, 0.0f);

	if (m_Sprite->GetWorldPos().y < 0.0f)
		m_Sprite->SetWorldPos(m_Sprite->GetWorldPos().x, m_Resolution.y - 1.0f, 0.0f);

	if (m_Sprite->GetWorldPos().y >= m_Resolution.y)
		m_Sprite->SetWorldPos(m_Sprite->GetWorldPos().x, 0.0f, 0.0f);
}
