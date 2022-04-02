#pragma once

#include "../../GameInfo.h"

class CStructuredBuffer
{
public:
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& buffer);
	~CStructuredBuffer();

private:
	std::string m_Name;
	D3D11_BUFFER_DESC m_Desc;
	ID3D11Buffer* m_Buffer;

	ID3D11ShaderResourceView* m_SRV;
	ID3D11UnorderedAccessView* m_UAV;

	unsigned int m_Size;
	unsigned int m_Count;

	int m_StructedBufferShaderType;
	int m_RegisterNum;
	bool m_Dynamic;

public:
	inline ID3D11Buffer* GetBuffer() const
	{
		return m_Buffer;
	}

	inline const std::string GetName() const
	{
		return m_Name;
	}

public:
	bool Init(const std::string& name, unsigned int size, unsigned int count, int registerNum, bool isDynamic = false,
		int structuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	void UpdateBuffer(void* data, int count);
	CStructuredBuffer* Clone();
	void SetShader();
	void ResetShader();
	void SetShader(int registerNum, int structuredBufferShaderType);
	void ResetShader(int registerNum, int structuredBufferShaderType);
	void CopyData(void* data);
	void CopyResource(CStructuredBuffer* buffer);
};

