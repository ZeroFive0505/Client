#pragma once

#include "ConstantBufferBase.h"

class CLandScapeConstantBuffer :
	public CConstantBufferBase
{
public:
	CLandScapeConstantBuffer();
	CLandScapeConstantBuffer(const CLandScapeConstantBuffer& buffer);
	virtual ~CLandScapeConstantBuffer();

protected:
	sLandScapeCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CLandScapeConstantBuffer* Clone();

public:
	inline void SetUVScaleX(float x)
	{
		m_BufferData.uvScale.x = x;
	}

	inline void SetUVScaleY(float y)
	{
		m_BufferData.uvScale.y = y;
	}

	inline void SetUVScale(float x, float y)
	{
		m_BufferData.uvScale = Vector2(x, y);
	}

	inline void SetUVScale(const Vector2 uv)
	{
		m_BufferData.uvScale = uv;
	}

	inline void SetSplatCount(int count)
	{
		m_BufferData.splatCount = count;
	}
};

