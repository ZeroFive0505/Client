#pragma once

#include "ConstantBufferBase.h"

class CColliderConstantBuffer :
	public CConstantBufferBase
{
public:
	CColliderConstantBuffer();
	CColliderConstantBuffer(const CColliderConstantBuffer& buffer);
	virtual ~CColliderConstantBuffer();

protected:
	sColliderCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CColliderConstantBuffer* Clone();

public:
	inline void SetColliderColor(const Vector4& color)
	{
		m_BufferData.color = color;
	}

	inline void SetColliderColor(float r, float g, float b, float a)
	{
		m_BufferData.color = Vector4(r, g, b, a);
	}

	inline void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
	}
};

