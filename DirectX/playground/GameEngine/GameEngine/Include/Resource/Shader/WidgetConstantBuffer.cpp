#include "WidgetConstantBuffer.h"
#include "ConstantBuffer.h"

CWidgetConstantBuffer::CWidgetConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.tint = Vector4::White;
}

CWidgetConstantBuffer::CWidgetConstantBuffer(const CWidgetConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CWidgetConstantBuffer::~CWidgetConstantBuffer()
{
}

bool CWidgetConstantBuffer::Init()
{
	SetConstantBuffer("WidgetCBuffer");

	return true;
}

void CWidgetConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CWidgetConstantBuffer* CWidgetConstantBuffer::Clone()
{
	return new CWidgetConstantBuffer(*this);
}
