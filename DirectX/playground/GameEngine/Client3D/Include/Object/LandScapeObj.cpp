#include "LandScapeObj.h"
#include "Scene/CameraManager.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Player.h"

CLandScapeObj::CLandScapeObj()
{
	SetTypeID<CLandScapeObj>();
}

CLandScapeObj::CLandScapeObj(const CLandScapeObj& obj) :
	CGameObject(obj)
{
	m_LandScape = (CLandScape*)FindComponent("LandScape");
}

CLandScapeObj::~CLandScapeObj()
{
}


bool CLandScapeObj::Init()
{
	m_LandScape = CreateComponent<CLandScape>("LandScape");

	m_LandScape->CreateLandScape("LandScape", 129, 129, TEXT("LandScape/height1.bmp"));

	m_LandScape->AddMaterial("LandScape");

	m_LandScape->SetUVScale(30.0f, 30.0f);
	m_LandScape->SetSplatCount(4);

	CInput::GetInst()->SetCallback("MousePick", KeyState_Down, this, &CLandScapeObj::RayCheck);

	return true;
}

void CLandScapeObj::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CLandScapeObj::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CLandScapeObj* CLandScapeObj::Clone()
{
	return new CLandScapeObj(*this);
}

void CLandScapeObj::RayCheck(float deltaTime)
{
	m_Scene->GetCameraManager()->RayCast();
	if (m_LandScape->RayVsPlane())
	{
		Vector3 intersection = m_LandScape->GetIntersection();

		Vector3 pos = m_LandScape->Bresenham((int)intersection.x, (int)intersection.z);

		m_Player->SetWorldPos(pos);
	}
}

void CLandScapeObj::SetPlayer(CPlayer* player)
{
	m_Player = player;
}
