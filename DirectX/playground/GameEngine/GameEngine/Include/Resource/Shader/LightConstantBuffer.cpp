#include "LightConstantBuffer.h"
#include "ConstantBuffer.h"

CLightConstantBuffer::CLightConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.lightColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_BufferData.distance = 10.0f;
	m_BufferData.innterAngle = 10.0f;
	m_BufferData.outterAngle = 15.0f;
	m_BufferData.attConst1 = 0.0f;
	m_BufferData.attConst2 = 0.0f;
	m_BufferData.attConst3 = 1.0f;
}

CLightConstantBuffer::CLightConstantBuffer(const CLightConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CLightConstantBuffer::~CLightConstantBuffer()
{
}

bool CLightConstantBuffer::Init()
{
	SetConstantBuffer("LightCBuffer");

	return true;
}

void CLightConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CLightConstantBuffer* CLightConstantBuffer::Clone()
{
	return new CLightConstantBuffer(*this);
}
