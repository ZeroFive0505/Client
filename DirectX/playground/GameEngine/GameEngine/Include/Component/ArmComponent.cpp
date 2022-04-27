#include "ArmComponent.h"
#include "../Device.h"

CArmComponent::CArmComponent()
{
	SetTypeID<CArmComponent>();
	m_Render = false;

	m_TargetDistance = 5.0f;
}

CArmComponent::CArmComponent(const CArmComponent& com) :
	CSceneComponent(com)
{
	m_TargetDistance = com.m_TargetDistance;
	m_Offset = com.m_Offset;
}

CArmComponent::~CArmComponent()
{
}

void CArmComponent::Start()
{
	CSceneComponent::Start();
}

bool CArmComponent::Init()
{
	return true;
}

void CArmComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (m_Parent)
	{
		Vector3 parentPos = m_Parent->GetWorldPos();

		Vector3 pos = parentPos - GetWorldAxis(AXIS_Z) * m_TargetDistance;

		SetWorldPos(pos + m_Offset);
	}
}

void CArmComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CArmComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CArmComponent::Render()
{
	CSceneComponent::Render();
}

void CArmComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CArmComponent* CArmComponent::Clone()
{
	return new CArmComponent(*this);
}

void CArmComponent::Save(FILE* pFile)
{
	fwrite(&m_TargetDistance, sizeof(float), 1, pFile);
	fwrite(&m_Offset, sizeof(Vector3), 1, pFile);

	CSceneComponent::Save(pFile);
}

void CArmComponent::Load(FILE* pFile)
{
	fread(&m_TargetDistance, sizeof(float), 1, pFile);
	fread(&m_Offset, sizeof(Vector3), 1, pFile);

	CSceneComponent::Load(pFile);
}
