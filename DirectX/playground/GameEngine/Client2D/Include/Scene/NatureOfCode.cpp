#include "NatureOfCode.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneResource.h"
#include "../Object/Enemy.h"
#include "../Object/PixelTest.h"
#include "../Object/Mover.h"
#include "../Object/Observer.h"
#include "../Object/Attractor.h"
#include "../Object/Liquid.h"
#include "Engine.h"
#include "Device.h"
#include "Resource/Mesh/Mesh.h"
#include "../Object/Line.h"
#include "../Object/Pendulum.h"
#include "../Object/Spring.h"
#include "../Object/Vehicle.h"
#include "../Object/FlowField.h"
#include "../Object/Path.h"
#include "Input.h"

static float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

CNatureOfCode::CNatureOfCode() :
	m_Attractor(nullptr),
	m_vecPendulum{},
	m_vecMover{}
{
	SetTypeID<CNatureOfCode>();
}

CNatureOfCode::~CNatureOfCode()
{
	SAFE_DELETE(m_Field);
	SAFE_DELETE(m_Path);
}

bool CNatureOfCode::Init()
{
	sResolution rs = CEngine::GetInst()->GetResolution();
	m_ElapsedTime = 0.0f;
	CObserver* observer = m_Scene->CreateGameObject<CObserver>("Observer");

	/*CMover* m = m_Scene->CreateGameObject<CMover>("Mover");

	CSpring* spring = m_Scene->CreateGameObject<CSpring>("Spring");

	spring->SetAnchor(Vector2((float)rs.width / 2.0f, (float)rs.height));

	spring->SetRestLength(400.0f);

	spring->SetSpringConstant(0.1f);

	m->SetWorldPos((float)rs.width / 2.0f, (float)rs.height / 2.0f, 0.0f);

	spring->ConnectMover(m);

	m_vecMover.push_back(m);


	m_FrictionConst = 1.5f;

	m_G = 5.0f;

	m_Radius = 50.0f;*/

	//m_Field = new CFlowField;

	//SetPlayerObject(observer);

	m_Path = new CPath;
	m_Path->Init();

	CInput::GetInst()->SetCallback<CNatureOfCode>("Spawn", KeyState_Down, this, &CNatureOfCode::SpawnVehicle);

	return true;
}

void CNatureOfCode::Update(float deltaTime)
{
	// ApplyGravity(deltaTime);
	// ApplyFriction(deltaTime);
	// ApplyAttract(deltaTime);
	// ApplyRotation(deltaTime);

	size_t size = m_vecVehicle.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecVehicle[i]->Update(deltaTime);
	}
}

void CNatureOfCode::PostUpdate(float deltaTime)
{
	size_t size = m_vecVehicle.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecVehicle[i]->PostUpdate(deltaTime);
	}
}

void CNatureOfCode::ApplyGravity(float deltaTime)
{
	size_t size = m_vecMover.size();
	for (size_t i = 0; i < size; i++)
	{
		if (m_vecMover[i])
		{
			Vector2 gravity = Vector2(0.0f, -1.0f * m_vecMover[i]->GetMass());
			m_vecMover[i]->ApplyForce(gravity * deltaTime);
		}
	}
}

void CNatureOfCode::ApplyFriction(float deltaTime)
{
	size_t size = m_vecMover.size();
	for (size_t i = 0; i < size; i++)
	{
		if (m_vecMover[i])
		{
			Vector2 dir = m_vecMover[i]->GetVelocity();
			dir.Normalize();

			Vector2 friction = dir * -m_FrictionConst;

			m_vecMover[i]->ApplyForce(friction * deltaTime);
		}
	}
}

void CNatureOfCode::ApplyAttract(float deltaTime)
{
	if (m_Attractor)
	{
		size_t size = m_vecMover.size();
		for (size_t i = 0; i < size; i++)
		{
			m_Attractor->Attract((CGameObject*)m_vecMover[i], deltaTime);
		}

		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				if (i == j)
					continue;

				float m1 = m_vecMover[i]->GetMass();
				float m2 = m_vecMover[j]->GetMass();

				Vector2 force;

				if (m1 > m2)
				{
					force = Vector2(m_vecMover[i]->GetWorldPos().x - m_vecMover[j]->GetWorldPos().x,
						m_vecMover[i]->GetWorldPos().y - m_vecMover[j]->GetWorldPos().y);
				}
				else
				{
					force = Vector2(m_vecMover[j]->GetWorldPos().x - m_vecMover[i]->GetWorldPos().x,
						m_vecMover[j]->GetWorldPos().y - m_vecMover[i]->GetWorldPos().y);
				}
				
				float dist = force.Length();

				dist = std::fmax(5.0f, std::fmin(dist, 100.0f));

				float strength = (m_G * m1 * m2) / (dist * dist);

				force.Normalize();

				force *= strength;

				m_vecMover[i]->ApplyForce(force * deltaTime);
			}
		}
	}
}

void CNatureOfCode::ApplyRotation(float deltaTime)
{
	size_t size = m_vecMover.size();

	for (size_t i = 0; i < size; i++)
	{
		Vector2 centerPos = m_vecMover[i]->GetCenterPos();

		m_vecMover[i]->AddAngularForceX(0.01f * (i + 1) * deltaTime);
		m_vecMover[i]->AddAngularForceY(0.01f * (i + 1) * deltaTime);

		float x = m_vecMover[i]->GetAmplitude().x * sinf(m_vecMover[i]->GetAnlge().x + m_ElapsedTime * m_vecMover[i]->GetAngularVelocity().x);
		float y = m_vecMover[i]->GetAmplitude().y * cosf(m_vecMover[i]->GetAnlge().y + m_ElapsedTime * m_vecMover[i]->GetAngularVelocity().y);

		m_vecMover[i]->SetWorldPos(centerPos.x + x, centerPos.y + y, 0.0f);
	}
}

void CNatureOfCode::RadiusUp(float deltaTime)
{
	m_Radius += deltaTime * 20.0f;
}

void CNatureOfCode::RadiusDown(float deltaTime)
{
	m_Radius -= deltaTime * 20.0f;
}

void CNatureOfCode::SpawnVehicle(float deltaTime)
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	size_t size = m_vecVehicle.size();
	CVehicle* vehicle = m_Scene->CreateGameObject<CVehicle>("Vehicle " + std::to_string((int)size + 1));

	vehicle->SetWorldPos(mousePos.x, mousePos.y, 0.0f);
	// vehicle->SetFlowField(m_Field);
	vehicle->SetPath(m_Path);

	m_vecVehicle.push_back(vehicle);
}
