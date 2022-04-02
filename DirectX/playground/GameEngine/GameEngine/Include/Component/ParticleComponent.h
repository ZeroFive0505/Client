#pragma once

#include "SceneComponent.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/ParticleUpdateShader.h"
#include "../Resource/Shader/ParticleRenderShader.h"
#include "../Resource/Shader/ParticleConstantBuffer.h"
#include "../Resource/Material/Material.h"

class CParticleComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CParticleComponent();
	CParticleComponent(const CParticleComponent& com);
	virtual ~CParticleComponent();

protected:
	// 파티클
	CSharedPtr<CParticle> m_Particle;
	// 구조화 버퍼
	std::vector<class CStructuredBuffer*> m_vecStructedBuffer;
	
	CSharedPtr<CMesh> m_Mesh;
	CSharedPtr<CMaterial> m_Material;
	CSharedPtr<CParticleUpdateShader> m_UpdateShader;
	CParticleConstantBuffer* m_CBuffer;
	sParticleInfo m_Info;
	sParticleInfoShared m_InfoShared;
	float m_SpawnTime;
	float m_SpawnTimeMax;

public:
	void SetParticle(const std::string& name);
	void SetParticle(CParticle* particle);
	void SetSpawnTime(float spawnTime);

public:
	inline void SetParticleMoveDir(float x, float y, float z)
	{
		m_Particle->SetMoveDir(Vector3(x, y, z));
	}

	inline void SetParticleMoveDir(const Vector3& dir)
	{
		m_Particle->SetMoveDir(dir);
	}

	inline void SetParticleMoveAngle(float x, float y, float z)
	{
		m_Particle->SetMoveAngle(Vector3(x, y, z));
	}

	inline void SetParticleMoveAngle(const Vector3& angle)
	{
		m_Particle->SetMoveAngle(angle);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CParticleComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

