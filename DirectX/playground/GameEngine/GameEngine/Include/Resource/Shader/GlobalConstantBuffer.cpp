#include "GlobalConstantBuffer.h"
#include "ConstantBuffer.h"

CGlobalConstantBuffer::CGlobalConstantBuffer() :
	m_BufferData{}
{
}

CGlobalConstantBuffer::CGlobalConstantBuffer(const CGlobalConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CGlobalConstantBuffer::~CGlobalConstantBuffer()
{
}

bool CGlobalConstantBuffer::Init()
{
	SetConstantBuffer("GlobalCBuffer");

	return true;
}

void CGlobalConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CGlobalConstantBuffer* CGlobalConstantBuffer::Clone()
{
	return new CGlobalConstantBuffer(*this);
}
