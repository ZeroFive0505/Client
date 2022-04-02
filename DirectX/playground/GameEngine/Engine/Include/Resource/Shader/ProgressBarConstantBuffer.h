#pragma once

#include "ConstantBufferBase.h"

class CProgressBarConstantBuffer :
	public CConstantBufferBase
{
public:
	CProgressBarConstantBuffer();
	CProgressBarConstantBuffer(const CProgressBarConstantBuffer& buffer);
	virtual ~CProgressBarConstantBuffer();

protected:
	sProgressBarCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CProgressBarConstantBuffer* Clone();

public:
	inline void SetPercent(float percent)
	{
		m_BufferData.percent = percent;
	}

	inline void SetDirection(ProgressBar_Direction direction)
	{
		m_BufferData.direction = (int)direction;
	}
};

