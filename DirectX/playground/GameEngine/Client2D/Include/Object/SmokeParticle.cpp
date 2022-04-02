#include "SmokeParticle.h"
#include "Component/ParticleComponent.h"

CSmokeParticle::CSmokeParticle()
{
    SetTypeID<CSmokeParticle>();
}

CSmokeParticle::CSmokeParticle(const CSmokeParticle& obj) :
    CGameObject(obj)
{
    m_Particle = (CParticleComponent*)FindComponent("SmokeParticle");
}

CSmokeParticle::~CSmokeParticle()
{
}

bool CSmokeParticle::Init()
{
    m_Particle = CreateComponent<CParticleComponent>("SmokeParticle");

    SetRootComponent(m_Particle);

    m_Particle->SetParticle("SmokeParticle");

    return true;
}

void CSmokeParticle::SetMoveDir(float x, float y, float z)
{
    m_Particle->SetParticleMoveDir(Vector3(x, y, z));
}

void CSmokeParticle::SetMoveDir(const Vector3& dir)
{
    m_Particle->SetParticleMoveDir(dir);
}

void CSmokeParticle::SetMoveAngle(float x, float y, float z)
{
    m_Particle->SetParticleMoveAngle(Vector3(x, y, z));
}

void CSmokeParticle::SetMoveAngle(const Vector3& angle)
{
    m_Particle->SetParticleMoveAngle(angle);
}

CSmokeParticle* CSmokeParticle::Clone()
{
    return new CSmokeParticle(*this);
}
