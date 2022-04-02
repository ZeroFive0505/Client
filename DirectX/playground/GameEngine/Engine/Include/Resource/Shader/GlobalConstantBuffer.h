#pragma once

#include "ConstantBufferBase.h"

class CGlobalConstantBuffer :
	public CConstantBufferBase
{
public:
	CGlobalConstantBuffer();
	CGlobalConstantBuffer(const CGlobalConstantBuffer& buffer);
	virtual ~CGlobalConstantBuffer();

protected:
	sGlobalCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CGlobalConstantBuffer* Clone();

public:
	inline void SetDeltaTime(float time)
	{
		m_BufferData.deltaTime = time;
	}

	inline void SetAccTime(float time)
	{
		m_BufferData.accTime = time;
	}

	inline void SetResolution(const sResolution& rs)
	{
		m_BufferData.resolution.x = (float)rs.width;
		m_BufferData.resolution.y = (float)rs.height;
	}

	inline void SetNoiseTextureResolution(float width, float height)
	{
		m_BufferData.noiseResolution.x = width;
		m_BufferData.noiseResolution.y = height;
	}
};

