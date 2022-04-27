#include "Line.h"

CLine::CLine()
{
	SetTypeID<CLine>();
}

CLine::CLine(const CLine& obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Line");
}

CLine::~CLine()
{
}

bool CLine::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Line");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("LineMesh");
	m_Mesh->GetMaterial()->SetShader("LineShader");
	m_Mesh->SetBaseColor(0.0f, 1.0f, 0.0f, 1.0f);

	return true;
}

void CLine::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CLine::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CLine* CLine::Clone()
{
	return new CLine(*this);
}
