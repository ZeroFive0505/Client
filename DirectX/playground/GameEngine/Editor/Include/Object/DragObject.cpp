#include "DragObject.h"
#include "Component/StaticMeshComponent.h"
#include "Animation/AnimationSequence2DInstance.h"

CDragObject::CDragObject()
{
}

CDragObject::CDragObject(const CDragObject& obj) :
	CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CDragObject::~CDragObject()
{
}

void CDragObject::SetStartPos(const Vector2& pos)
{
	m_StartPos = pos;

	m_Mesh->SetWorldPos(m_StartPos.x, m_StartPos.y, 0.0f);
}

void CDragObject::SetEndPos(const Vector2& pos)
{
	m_EndPos = pos;

	m_Mesh->SetWorldScale(m_EndPos.x - m_StartPos.x, m_EndPos.y - m_StartPos.y, 1.0f);
}

void CDragObject::SetStartPosX(float x)
{
	m_StartPos.x = x;

	m_Mesh->SetWorldPos(m_StartPos.x, m_StartPos.y, 0.0f);
}

void CDragObject::SetStartPosY(float y)
{
	m_StartPos.y = y;

	m_Mesh->SetWorldPos(m_StartPos.x, m_StartPos.y, 0.0f);
}

void CDragObject::SetEndPosX(float x)
{
	m_EndPos.x = x;

	m_Mesh->SetWorldPos(m_EndPos.x, m_EndPos.y, 0.0f);
}

void CDragObject::SetEndPosY(float y)
{
	m_EndPos.y = y;
	
	m_Mesh->SetWorldPos(m_EndPos.x, m_EndPos.y, 0.0f);
}

bool CDragObject::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);
	m_Mesh->SetLayerName("DragLayer");
	m_Mesh->SetMesh("FrameRect");
	m_Mesh->GetMaterial()->SetShader("PosMeshShader");
	m_Mesh->SetPivot(0.0f, 1.0f, 0.0f);
	m_Mesh->SetBaseColor(0.0f, 1.0f, 0.0f, 1.0f);

	return true;
}

void CDragObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CDragObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CDragObject* CDragObject::Clone()
{
	return new CDragObject(*this);
}
