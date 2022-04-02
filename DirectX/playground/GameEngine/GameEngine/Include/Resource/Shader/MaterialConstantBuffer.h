#pragma once

#include "ConstantBufferBase.h"

class CMaterialConstantBuffer :
	public CConstantBufferBase
{
public:
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer);
	virtual ~CMaterialConstantBuffer();

protected:
	sMaterialCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CMaterialConstantBuffer* Clone();

public:
	inline void SetBaseColor(const Vector4& baseColor)
	{
		m_BufferData.baseColor = baseColor;
	}

	inline void SetOpacity(float opacity)
	{
		m_BufferData.opacity = opacity;
	}

	inline void SetPaperBurn(bool enable)
	{
		m_BufferData.paperBurnEnable = enable ? 1 : 0;
	}
};

