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

	inline void SetAmbientColor(const Vector4& color)
	{
		m_BufferData.ambientColor = color;
	}

	inline void SetSpecularColor(const Vector4& color)
	{
		m_BufferData.specularColor = color;
	}

	inline void SetEmissiveColor(const Vector4& color)
	{
		m_BufferData.emissiveColor = color;
	}

	inline void SetOpacity(float opacity)
	{
		m_BufferData.opacity = opacity;
	}

	inline void SetPaperBurn(bool enable)
	{
		m_BufferData.paperBurnEnable = enable ? 1 : 0;
	}

	inline void SetBump(bool enable)
	{
		m_BufferData.bumpEnable = enable ? 1 : 0;
	}

	inline void SetAnimation3D(bool enable)
	{
		m_BufferData.animation3DEnable = enable ? 1 : 0;
	}

	inline void SetSpecularTex(bool enable)
	{
		m_BufferData.specularTex = enable ? 1 : 0;
	}

	inline void SetEmissiveTex(bool enable)
	{
		m_BufferData.emissiveTex = enable ? 1 : 0;
	}
};

