#pragma once

#include "ConstantBufferBase.h"

class CTransformConstantBuffer :
	public CConstantBufferBase
{
public:
	CTransformConstantBuffer();
	CTransformConstantBuffer(const CTransformConstantBuffer& buffer);
	virtual ~CTransformConstantBuffer();

protected:
	sTransformCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CTransformConstantBuffer* Clone();

public:
	inline void SetWorldMatrix(const Matrix& matWorld)
	{
		m_BufferData.matWorld = matWorld;
	}

	inline void SetViewMatrix(const Matrix& matView)
	{
		m_BufferData.matView = matView;
	}

	inline void SetProjMatrix(const Matrix& matProj)
	{
		m_BufferData.matProj = matProj;
	}

	inline void SetPivot(const Vector3& pivot)
	{
		m_BufferData.pivot = pivot;
	}

	void SetMeshSize(const Vector3& size)
	{
		m_BufferData.meshSize = size;
	}
};

