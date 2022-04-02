#pragma once

#include "../../Ref.h"

/// <summary>
/// ��� ������ ������ ������ Ŭ����.
/// </summary>
class CConstantBuffer :
	public CRef
{
	friend class CShaderManager;

private:
	CConstantBuffer();
	~CConstantBuffer();

private:
	// ���� ����
	ID3D11Buffer* m_Buffer;
	int m_Size;
	int m_RegisterNum;
	int m_ConstantBufferShaderType;

public:
	bool Init(int size, int registerNum, int constantBufferShaderType);
	void UpdateBuffer(void* data);
};

