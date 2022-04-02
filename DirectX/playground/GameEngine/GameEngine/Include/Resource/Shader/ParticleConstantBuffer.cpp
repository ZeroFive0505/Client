#include "ParticleConstantBuffer.h"
#include "ConstantBuffer.h"

CParticleConstantBuffer::CParticleConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.spawnCountMax = 100;
	m_BufferData.colorMin = Vector4(1.f, 1.f, 1.f, 1.f);
	m_BufferData.colorMax = Vector4(1.f, 1.f, 1.f, 1.f);
	m_BufferData.lifeTimeMin = 0.5f;
	m_BufferData.lifeTimeMax = 1.f;
	m_BufferData.scaleMin = Vector3(30.f, 30.f, 1.f);
	m_BufferData.scaleMax = Vector3(30.f, 30.f, 1.f);
	m_BufferData.speedMin = 1.f;
	m_BufferData.speedMax = 3.f;
	m_BufferData.startMin = Vector3(-10.f, -10.f, 0.f);
	m_BufferData.startMax = Vector3(10.f, 10.f, 0.f);
}

CParticleConstantBuffer::CParticleConstantBuffer(const CParticleConstantBuffer& buffer) : 
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CParticleConstantBuffer::~CParticleConstantBuffer()
{
}

bool CParticleConstantBuffer::Init()
{
	SetConstantBuffer("ParticleCBuffer");

	return true;
}

void CParticleConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CParticleConstantBuffer* CParticleConstantBuffer::Clone()
{
	return new CParticleConstantBuffer(*this);
}
