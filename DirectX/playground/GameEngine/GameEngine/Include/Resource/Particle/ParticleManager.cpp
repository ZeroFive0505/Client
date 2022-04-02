#include "ParticleManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
}

bool CParticleManager::Init()
{
	return true;
}

bool CParticleManager::CreateParticle(const std::string& name)
{
	CParticle* particle = new CParticle;

	particle->SetName(name);

	if (!particle->Init())
	{
		SAFE_RELEASE(particle);
		return false;
	}

	m_mapParticle.insert(std::make_pair(name, particle));

	return true;
}

CParticle* CParticleManager::FindParticle(const std::string& name)
{
	auto iter = m_mapParticle.find(name);

	if (iter == m_mapParticle.end())
		return nullptr;

	return iter->second;
}

void CParticleManager::ReleaseParticle(const std::string& name)
{
	auto iter = m_mapParticle.find(name);

	if (iter != m_mapParticle.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapParticle.erase(iter);
	}
}
