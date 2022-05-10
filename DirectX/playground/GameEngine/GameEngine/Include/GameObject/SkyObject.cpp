#include "SkyObject.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material/Material.h"

CSkyObject::CSkyObject()
{
	SetTypeID<CSkyObject>();
}

CSkyObject::CSkyObject(const CSkyObject& obj) :
	CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CSkyObject::~CSkyObject()
{
}

bool CSkyObject::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("SpherePos");


	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("SkyMaterial");

	material->SetRenderState("FrontFaceCull");
	material->SetRenderState("SkyDepth");

	m_Mesh->AddMaterial(material);

	m_Mesh->SetLayerName("Back");

	m_Mesh->SetRelativeScale(100000.0f, 100000.0f, 100000.0f);

	return true;
}

void CSkyObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CSkyObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CSkyObject* CSkyObject::Clone()
{
	return new CSkyObject(*this);
}
