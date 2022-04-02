#pragma once

#include "ConstantBufferBase.h"

/// <summary>
/// �ִϸ��̼� ������۷� �ؽ��Ŀ��� �ִϸ��̼� UV�� ��ǥ�� ���̴��� �����ֱ� ���ؼ� ���ȴ�.
/// </summary>
class CAnimation2DConstantBuffer :
	public CConstantBufferBase
{
public:
	CAnimation2DConstantBuffer();
	CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& buffer);
	virtual ~CAnimation2DConstantBuffer();

protected:
	sAnimation2DCBuffer m_BufferData;


public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CAnimation2DConstantBuffer* Clone();

public:
	inline void SetAnimation2DType(Image_Type type)
	{
		m_BufferData.Animation2DType = (int)type;
	}

	inline void SetStartUV(const Vector2& startUV)
	{
		m_BufferData.Animation2DStartUV = startUV;
	}

	inline void SetEndUV(const Vector2& endUV)
	{
		m_BufferData.Animation2DEndUV = endUV;
	}

	inline void SetAnimationHrizontalFlip(bool flip)
	{
		m_BufferData.AnimationHorizontalFlip = flip ? 1 : 0;
	}

	inline void SetAnimationSize(const Vector2& size)
	{
		m_BufferData.AnimationSize = size;
	}
};

