#include "BubbleParticle.h"
#include "Component/ParticleComponent.h"

CBubbleParticle::CBubbleParticle()
{
	SetTypeID<CBubbleParticle>();
}

CBubbleParticle::CBubbleParticle(const CBubbleParticle& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CBubbleParticle::~CBubbleParticle()
{
}

bool CBubbleParticle::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Particle");
	
	SetRootComponent(m_Particle);

	m_Particle->SetRelativePos(200.0f, 50.0f, 0.0f);

	m_Particle->SetParticle("Bubble");

	return true;
}

CBubbleParticle* CBubbleParticle::Clone()
{
	return new CBubbleParticle(*this);
}
