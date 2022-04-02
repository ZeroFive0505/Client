#pragma once

#include "../../Ref.h"

/// <summary>
/// 상수 버퍼의 정보를 가지는 클래스.
/// </summary>
class CConstantBuffer :
	public CRef
{
	friend class CShaderManager;

private:
	CConstantBuffer();
	~CConstantBuffer();

private:
	// 버퍼 변수
	ID3D11Buffer* m_Buffer;
	int m_Size;
	int m_RegisterNum;
	int m_ConstantBufferShaderType;

public:
	bool Init(int size, int registerNum, int constantBufferShaderType);
	void UpdateBuffer(void* data);
};

