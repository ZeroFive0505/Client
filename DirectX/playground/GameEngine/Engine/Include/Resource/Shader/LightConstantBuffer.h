#pragma once

#include "ConstantBufferBase.h"

class CLightConstantBuffer :
	public CConstantBufferBase
{
public:
	CLightConstantBuffer();
	CLightConstantBuffer(const CLightConstantBuffer& buffer);
	virtual ~CLightConstantBuffer();

protected:
	sLightBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CLightConstantBuffer* Clone();

public:
	inline Light_Type GetLightType() const
	{
		return (Light_Type)m_BufferData.lightType;
	}

	inline float GetLightDistance() const
	{
		return m_BufferData.distance;
	}

public:
	inline void SetColor(const Vector4& color)
	{
		m_BufferData.lightColor = color;
	}

	inline void SetLightType(Light_Type lightType)
	{
		m_BufferData.lightType = (int)lightType;
	}

	inline void SetPos(const Vector3& pos)
	{
		m_BufferData.pos = pos;
	}

	inline void SetDir(const Vector3& dir)
	{
		m_BufferData.dir = dir;
	}

	inline void SetDistance(float distance)
	{
		m_BufferData.distance = distance;
	}

	inline void SetInnerAngle(float angle)
	{
		m_BufferData.innterAngle = angle;
	}

	inline void SetOutterAngle(float angle)
	{
		m_BufferData.outterAngle = angle;
	}

	inline void SetAttConst1(float c)
	{
		m_BufferData.attConst1 = c;
	}

	inline void SetAttConst2(float c)
	{
		m_BufferData.attConst2 = c;
	}

	inline void SetAttConst3(float c)
	{
		m_BufferData.attConst3 = c;
	}
};

