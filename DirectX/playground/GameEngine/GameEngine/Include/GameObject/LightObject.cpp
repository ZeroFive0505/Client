#include "LightObject.h"

CLightObject::CLightObject()
{
	SetTypeID<CLightObject>();
}

CLightObject::CLightObject(const CLightObject& obj) :
	CGameObject(obj)
{
	m_Light = (CLightComponent*)FindComponent("Light");
}

CLightObject::~CLightObject()
{
}

bool CLightObject::Init()
{
	m_Light = CreateComponent<CLightComponent>("Light");

	return true;
}

void CLightObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CLightObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CLightObject* CLightObject::Clone()
{
	return new CLightObject(*this);
}
