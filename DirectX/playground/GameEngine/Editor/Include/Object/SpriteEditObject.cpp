#include "SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Engine.h"

CSpriteEditObject::CSpriteEditObject()
{
}

CSpriteEditObject::CSpriteEditObject(const CSpriteEditObject& obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("SpriteEdit");
}

CSpriteEditObject::~CSpriteEditObject()
{
}

bool CSpriteEditObject::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SpriteEdit");

	SetRootComponent(m_Sprite);

	m_Sprite->SetPivot(0.0f, 1.0f, 0.0f);

	sResolution rs = CEngine::GetInst()->GetResolution();

	m_Sprite->SetRelativePos(0.0f, (float)rs.height, 0.0f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	return true;
}

void CSpriteEditObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CSpriteEditObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CSpriteEditObject* CSpriteEditObject::Clone()
{
	return new CSpriteEditObject(*this);
}
