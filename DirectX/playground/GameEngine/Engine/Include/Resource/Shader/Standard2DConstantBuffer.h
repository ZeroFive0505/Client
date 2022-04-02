#pragma once

#include "ConstantBufferBase.h"

/// <summary>
/// 애니메이션 온 오프를 담당하는 버퍼
/// </summary>
class CStandard2DConstantBuffer :
	public CConstantBufferBase
{
public:
	CStandard2DConstantBuffer();
	CStandard2DConstantBuffer(const CStandard2DConstantBuffer& buffer);
	virtual ~CStandard2DConstantBuffer();

protected:
	sStandard2DCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CStandard2DConstantBuffer* Clone();

public:
	inline void SetAnimation2DEnable(bool enable)
	{
		m_BufferData.AnimationEnable = enable ? 1 : 0;
	}
};

