#pragma once

#include "ConstantBufferBase.h"

class CPaperBurnConstantBuffer :
	public CConstantBufferBase
{
public:
	CPaperBurnConstantBuffer();
	CPaperBurnConstantBuffer(const CPaperBurnConstantBuffer& buffer);
	virtual ~CPaperBurnConstantBuffer();

protected:
	sPaperBurnCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CPaperBurnConstantBuffer* Clone();

public:
	inline void SetInColor(const Vector4& color)
	{
		m_BufferData.inlineColor = color;
	}

	inline void SetInColor(float r, float g, float b, float a)
	{
		m_BufferData.inlineColor = Vector4(r, g, b, a);
	}

	inline void SetOutColor(const Vector4& color)
	{
		m_BufferData.outlineColor = color;
	}

	inline void SetOutColor(float r, float g, float b, float a)
	{
		m_BufferData.outlineColor = Vector4(r, g, b, a);
	}

	inline void SetCenterColor(const Vector4& color)
	{
		m_BufferData.centerlineColor = color;
	}

	inline void SetCenterColor(float r, float g, float b, float a)
	{
		m_BufferData.centerlineColor = Vector4(r, g, b, a);
	}

	inline void SetFilter(float burn)
	{
		m_BufferData.filter = burn;
	}

	inline void AddFilter(float filter)
	{
		m_BufferData.filter += filter;
	}

	inline void SetInverse(bool inverse)
	{
		m_BufferData.inverse = inverse ? 1 : 0;
	}

	inline void SetInFilter(float filter)
	{
		m_BufferData.inFilter = filter;
	}

	inline void SetOutFilter(float filter)
	{
		m_BufferData.outFilter = filter;
	}

	inline void SetCenterFilter(float filter)
	{
		m_BufferData.centerFilter = filter;
	}
};

