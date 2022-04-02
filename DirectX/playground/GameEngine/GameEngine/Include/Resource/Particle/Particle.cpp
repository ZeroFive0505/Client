#include "Particle.h"
#include "../Shader/StructuredBuffer.h"
#include "../Shader/ShaderManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"

CParticle::CParticle() :
	m_CBuffer(nullptr),
	m_SpawnTime(0.001f),
	m_2D(true),
	m_SpawnCountMax(100)
{
}

CParticle::CParticle(const CParticle& particle)
{
	m_CBuffer = particle.m_CBuffer->Clone();
	m_UpdateShader = particle.m_UpdateShader->Clone();
}

CParticle::~CParticle()
{
	size_t bufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

bool CParticle::Init()
{
	m_CBuffer = new CParticleConstantBuffer;

	if (!m_CBuffer->Init())
		return false;

	if (m_Scene)
		m_UpdateShader = (CParticleUpdateShader*)m_Scene->GetSceneResource()->FindShader("ParticleUpdateShader");
	else
		m_UpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader("ParticleUpdateShader");

	// 파티클의 구조화 버퍼를 추가한다.
	// 파티클 하나하나의 정보를 담는 버퍼 0번 레지스터
	AddStructuredBuffer("ParticleInfo", sizeof(sParticleInfo), m_SpawnCountMax, 0);
	// 파티클이 공유하는 정보를 담는 버퍼 1번 레지스터
	AddStructuredBuffer("ParticleInfoShared", sizeof(sParticleInfoShared), 1, 1);

	return true;
}

void CParticle::AddStructuredBuffer(const std::string& name, unsigned int size, unsigned int count, int registerNum, bool isDynamic, int structuredBufferShaderType)
{
	CStructuredBuffer* buffer = new CStructuredBuffer;


	if (!buffer->Init(name, size, count, registerNum, isDynamic, structuredBufferShaderType))
	{
		SAFE_DELETE(buffer);
		return;
	}

	m_vecStructuredBuffer.push_back(buffer);
}

bool CParticle::ResizeBuffer(const std::string& name, unsigned int size, unsigned int count, int registerNum, bool isDynamic, int structuredBufferShaderType)
{
	size_t bufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		if (m_vecStructuredBuffer[i]->GetName() == name)
		{
			m_vecStructuredBuffer[i]->Init(name, size, count, registerNum, isDynamic, structuredBufferShaderType);

			return true;
		}
	}

	return false;
}

void CParticle::CloneStructedBuffer(std::vector<CStructuredBuffer*>& vecBuffer)
{
	size_t bufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		CStructuredBuffer* buffer = m_vecStructuredBuffer[i]->Clone();

		vecBuffer.push_back(buffer);
	}
}

void CParticle::SetSpawnCountMax(unsigned int count)
{
	m_CBuffer->SetSpawnCountMax(count);

	m_SpawnCountMax = count;

	ResizeBuffer("ParticleInfo", sizeof(sParticleInfo), m_SpawnCountMax, 0);
}