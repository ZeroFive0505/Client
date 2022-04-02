#include "StructuredBuffer.h"
#include "../../Device.h"

CStructuredBuffer::CStructuredBuffer() :
	m_Buffer(nullptr),
	m_SRV(nullptr),
	m_UAV(nullptr),
	m_Desc{}
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& buffer)
{
	*this = buffer;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = m_Count;

	CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &srvDesc, &m_SRV);

	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.NumElements = m_Count;

		CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &uavDesc, &m_UAV);
	}
}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
}

bool CStructuredBuffer::Init(const std::string& name, unsigned int size, unsigned int count, int registerNum, bool isDynamic, int structuredBufferShaderType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Dynamic = isDynamic;
	m_Name = name;
	m_Size = size;
	m_Count = count;
	m_RegisterNum = registerNum;
	m_StructedBufferShaderType = structuredBufferShaderType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	if (m_Dynamic)
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = m_Count;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &srvDesc, &m_SRV)))
		return false;

	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.NumElements = m_Count;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &uavDesc, &m_UAV)))
			return false;
	}

	return true;
}

void CStructuredBuffer::UpdateBuffer(void* data, int count)
{
	if (!m_Dynamic)
		return;

	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, data, m_Size * count);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}

CStructuredBuffer* CStructuredBuffer::Clone()
{
	return new CStructuredBuffer(*this);
}

void CStructuredBuffer::SetShader()
{
	if (m_Dynamic)
	{
		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_RegisterNum, 1, &m_SRV);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_RegisterNum, 1, &m_SRV);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_RegisterNum, 1, &m_SRV);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Hull)
			CDevice::GetInst()->GetContext()->HSGetShaderResources(m_RegisterNum, 1, &m_SRV);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_RegisterNum, 1, &m_SRV);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_RegisterNum, 1, &m_SRV);
	}
	else
	{
		unsigned int count = -1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_RegisterNum, 1, &m_UAV, &count);
	}
}

void CStructuredBuffer::ResetShader()
{
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_RegisterNum, 1, &srv);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_RegisterNum, 1, &srv);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_RegisterNum, 1, &srv);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Hull)
			CDevice::GetInst()->GetContext()->HSGetShaderResources(m_RegisterNum, 1, &srv);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_RegisterNum, 1, &srv);

		if (m_StructedBufferShaderType & (int)Buffer_Shader_Type::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_RegisterNum, 1, &srv);
	}
	else
	{
		unsigned int count = -1;
		ID3D11UnorderedAccessView* uav = nullptr;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_RegisterNum, 1, &uav, &count);
	}
}

void CStructuredBuffer::SetShader(int registerNum, int structuredBufferShaderType)
{
	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(registerNum, 1, &m_SRV);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(registerNum, 1, &m_SRV);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetShaderResources(registerNum, 1, &m_SRV);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSGetShaderResources(registerNum, 1, &m_SRV);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetShaderResources(registerNum, 1, &m_SRV);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetShaderResources(registerNum, 1, &m_SRV);
}

void CStructuredBuffer::ResetShader(int registerNum, int structuredBufferShaderType)
{
	ID3D11ShaderResourceView* srv = nullptr;

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(registerNum, 1, &srv);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(registerNum, 1, &srv);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetShaderResources(registerNum, 1, &srv);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSGetShaderResources(registerNum, 1, &srv);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetShaderResources(registerNum, 1, &srv);

	if (structuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetShaderResources(registerNum, 1, &srv);
}

void CStructuredBuffer::CopyData(void* data)
{
	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_READ, 0, &Map);

	memcpy(data, Map.pData, m_Size * m_Count);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}

void CStructuredBuffer::CopyResource(CStructuredBuffer* buffer)
{
	CDevice::GetInst()->GetContext()->CopyResource(buffer->m_Buffer, m_Buffer);
}
