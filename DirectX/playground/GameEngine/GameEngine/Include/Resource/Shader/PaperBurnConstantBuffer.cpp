#include "PaperBurnConstantBuffer.h"
#include "ConstantBuffer.h"

CPaperBurnConstantBuffer::CPaperBurnConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.outlineColor = Vector4(0.3f, 0.3f, 0.3f, 0.5f);
	m_BufferData.inlineColor = Vector4(0.8314f, 0.3922f, 0.f, 1.f);
	m_BufferData.centerlineColor = Vector4(0.6667f, 0.31378f, 0.f, 1.f);
	m_BufferData.outFilter = 0.1f;
	m_BufferData.centerFilter = 0.07f;
	m_BufferData.inFilter = 0.05f;
}

CPaperBurnConstantBuffer::CPaperBurnConstantBuffer(const CPaperBurnConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CPaperBurnConstantBuffer::~CPaperBurnConstantBuffer()
{
}

bool CPaperBurnConstantBuffer::Init()
{
	SetConstantBuffer("PaperBurnCBuffer");

	return true;
}

void CPaperBurnConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CPaperBurnConstantBuffer* CPaperBurnConstantBuffer::Clone()
{
	return new CPaperBurnConstantBuffer(*this);
}
