#include "LandScapeConstantBuffer.h"
#include "ConstantBuffer.h"

CLandScapeConstantBuffer::CLandScapeConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.uvScale = Vector2(1.0f, 1.0f);
}

CLandScapeConstantBuffer::CLandScapeConstantBuffer(const CLandScapeConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CLandScapeConstantBuffer::~CLandScapeConstantBuffer()
{
}

bool CLandScapeConstantBuffer::Init()
{
	SetConstantBuffer("LandScapeCBuffer");

	return true;
}

void CLandScapeConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CLandScapeConstantBuffer* CLandScapeConstantBuffer::Clone()
{
	return new CLandScapeConstantBuffer(*this);
}
