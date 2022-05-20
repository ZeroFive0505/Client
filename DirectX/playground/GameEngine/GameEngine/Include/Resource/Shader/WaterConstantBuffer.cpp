#include "WaterConstantBuffer.h"
#include "ConstantBuffer.h"

CWaterConstantBuffer::CWaterConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.uvScale = Vector2(1.0f, 1.0f);
	m_BufferData.scrollSpeed = Vector2(0.1f, 0.1f);
}

CWaterConstantBuffer::CWaterConstantBuffer(const CWaterConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CWaterConstantBuffer::~CWaterConstantBuffer()
{
}

bool CWaterConstantBuffer::Init()
{
	SetConstantBuffer("WaterPlane");

	return true;
}

void CWaterConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CWaterConstantBuffer* CWaterConstantBuffer::Clone()
{
	return new CWaterConstantBuffer(*this);
}
