#include "ConstantBufferBase.h"
#include "ConstantBuffer.h"
#include "../ResourceManager.h"

CConstantBufferBase::CConstantBufferBase()
{
}

CConstantBufferBase::CConstantBufferBase(const CConstantBufferBase& buffer)
{
	m_Buffer = buffer.m_Buffer;
}

CConstantBufferBase::~CConstantBufferBase()
{
}

void CConstantBufferBase::SetConstantBuffer(const std::string& name)
{
	m_Buffer = CResourceManager::GetInst()->FindConstantBuffer(name);
}
