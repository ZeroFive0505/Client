#pragma once

#include "ConstantBufferBase.h"

class CParticleConstantBuffer :
	public CConstantBufferBase
{
public:
	CParticleConstantBuffer();
	CParticleConstantBuffer(const CParticleConstantBuffer& buffer);
	virtual ~CParticleConstantBuffer();

protected:
	sParticleCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CParticleConstantBuffer* Clone();

public:
	inline const Vector3& GetStartMin() const
	{
		return m_BufferData.startMin;
	}

	inline const Vector3& GetStartMax() const
	{
		return m_BufferData.startMax;
	}

	inline int GetSpawnCount() const
	{
		return m_BufferData.spawnCountMax;
	}
	
public:
	inline void SetSpawnEnable(unsigned int enable)
	{
		m_BufferData.spawnEnable = enable;
	}

	inline void SetStartMin(const Vector3& startMin)
	{
		m_BufferData.startMin = startMin;
	}

	inline void SetStartMax(const Vector3& startMax)
	{
		m_BufferData.startMax = startMax;
	}

	inline void SetSpawnCountMax(unsigned int count)
	{
		m_BufferData.spawnCountMax = count;
	}

	inline void SetScaleMin(const Vector3& scaleMin)
	{
		m_BufferData.scaleMin = scaleMin;
	}

	inline void SetScaleMax(const Vector3& scaleMax)
	{
		m_BufferData.scaleMax = scaleMax;
	}

	inline void SetLifeTimeMin(float minLifeTime)
	{
		m_BufferData.lifeTimeMin = minLifeTime;
	}

	inline void SetLifeTimeMax(float maxLifeTime)
	{
		m_BufferData.lifeTimeMax = maxLifeTime;
	}

	inline void SetColorMin(const Vector4& colorMin)
	{
		m_BufferData.colorMin = colorMin;
	}

	inline void SetColorMax(const Vector4& colorMax)
	{
		m_BufferData.colorMax = colorMax;
	}

	inline void SetSpeedMin(float speedMin)
	{
		m_BufferData.speedMin = speedMin;
	}

	inline void SetSpeedMax(float speedMax)
	{
		m_BufferData.speedMax = speedMax;
	}

	inline void SetMove(bool isMoving)
	{
		m_BufferData.isMoving = isMoving ? 1 : 0;
	}
	
	inline void SetGravity(bool gravityEnable)
	{
		m_BufferData.gravityEnable = gravityEnable ? 1 : 0;
	}

	inline void SetMoveDir(const Vector3& moveDir)
	{
		m_BufferData.moveDir = moveDir;
	}

	inline void Set2D(bool is2D)
	{
		m_BufferData.is2D = is2D ? 1 : 0;
	}

	inline void SetMoveAngle(const Vector3& moveAngle)
	{
		m_BufferData.moveAngle = moveAngle;
	}
};

