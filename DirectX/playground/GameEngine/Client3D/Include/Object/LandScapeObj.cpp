#include "LandScapeObj.h"

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
