#include "Weapon.h"

CWeapon::CWeapon()
{
	SetTypeID<CWeapon>();
}

CWeapon::CWeapon(const CWeapon& obj) :
	CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CWeapon::~CWeapon()
{
}

bool CWeapon::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("Blade");

	return true;
}

void CWeapon::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CWeapon::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CWeapon* CWeapon::Clone()
{
	return new CWeapon(*this);
}
