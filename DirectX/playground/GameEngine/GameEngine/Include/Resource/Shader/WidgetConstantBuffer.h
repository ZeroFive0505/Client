#pragma once

#include "ConstantBufferBase.h"

class CWidgetConstantBuffer :
	public CConstantBufferBase
{
public:
	CWidgetConstantBuffer();
	CWidgetConstantBuffer(const CWidgetConstantBuffer& buffer);
	virtual ~CWidgetConstantBuffer();

protected:
	sWidgetCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CWidgetConstantBuffer* Clone();

public:
	inline void SetTint(const Vector4& color)
	{
		m_BufferData.tint = color;
	}

	inline void SetTint(float r, float g, float b, float a)
	{
		m_BufferData.tint = Vector4(r, g, b, a);
	}

	inline void SetWP(const Matrix& matWP)
	{
		m_BufferData.matWP = matWP;
	}

	inline void SetUseTexture(bool use)
	{
		m_BufferData.useTexture = use ? 1 : 0;
	}

	inline void SetAnimType(int type)
	{
		m_BufferData.widgetAnimType = type;
	}

	inline void SetStartUV(const Vector2& startUV)
	{
		m_BufferData.widgetAnimStartUV = startUV;
	}

	inline void SetEndUV(const Vector2& endUV)
	{
		m_BufferData.widgetAnimEndUV = endUV;
	}

	inline void SetAnimEnable(bool enable)
	{
		m_BufferData.widgetAnimEnable = enable ? 1 : 0;
	}

	inline void SetOpacity(float opacity)
	{
		m_BufferData.opacity = opacity;
	}
};

