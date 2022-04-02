#pragma once

#include "Particle.h"

class CParticleManager
{
	friend class CResourceManager;

private:
	CParticleManager();
	~CParticleManager();

private:
	std::unordered_map<std::string, CSharedPtr<CParticle>> m_mapParticle;

public:
	bool Init();
	bool CreateParticle(const std::string& name);
	CParticle* FindParticle(const std::string& name);
	void ReleaseParticle(const std::string& name);
};

