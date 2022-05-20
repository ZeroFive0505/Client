#pragma once

#include "ConstantBufferBase.h"

class CWaterConstantBuffer :
	public CConstantBufferBase
{
public:
	CWaterConstantBuffer();
	CWaterConstantBuffer(const CWaterConstantBuffer& buffer);
	virtual ~CWaterConstantBuffer();

private:
	sWaterPlaneCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CWaterConstantBuffer* Clone();

public:
	inline void SetUVScale(const Vector2& scale)
	{
		m_BufferData.uvScale = scale;
	}

	inline void SetUVScale(float x, float y)
	{
		m_BufferData.uvScale = Vector2(x, y);
	}

	inline void SetScrollSpeed(const Vector2& speed)
	{
		m_BufferData.scrollSpeed = speed;
	}

	inline void SetScrollSpeed(float x, float y)
	{
		m_BufferData.scrollSpeed = Vector2(x, y);
	}
};

