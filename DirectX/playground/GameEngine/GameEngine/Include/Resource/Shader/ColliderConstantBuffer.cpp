#include "ColliderConstantBuffer.h"
#include "ConstantBuffer.h"

CColliderConstantBuffer::CColliderConstantBuffer() :
    m_BufferData{}
{
}

CColliderConstantBuffer::CColliderConstantBuffer(const CColliderConstantBuffer& buffer) :
    CConstantBufferBase(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CColliderConstantBuffer::~CColliderConstantBuffer()
{
}

bool CColliderConstantBuffer::Init()
{
    SetConstantBuffer("ColliderCBuffer");

    return true;
}

void CColliderConstantBuffer::UpdateCBuffer()
{
    m_Buffer->UpdateBuffer(&m_BufferData);
}

CColliderConstantBuffer* CColliderConstantBuffer::Clone()
{
    return new CColliderConstantBuffer(*this);
}
