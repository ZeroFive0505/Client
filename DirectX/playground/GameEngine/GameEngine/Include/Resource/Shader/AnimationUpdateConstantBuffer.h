#pragma once

#include "ConstantBufferBase.h"

class CAnimationUpdateConstantBuffer :
	public CConstantBufferBase
{
public:
	CAnimationUpdateConstantBuffer();
	CAnimationUpdateConstantBuffer(const CAnimationUpdateConstantBuffer& buffer);
	virtual ~CAnimationUpdateConstantBuffer();

protected:
	sAnimationCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CAnimationUpdateConstantBuffer* Clone();

public:
	inline void SetBoneCount(int count)
	{
		m_BufferData.boneCount = count;
	}

	inline void SetCurrentFrame(int currentFrame)
	{
		m_BufferData.currentFrame = currentFrame;
	}

	inline void SetNextFrame(int nextFrame)
	{
		m_BufferData.nextFrame = nextFrame;
	}

	inline void SetRatio(float ratio)
	{
		m_BufferData.ratio = ratio;
	}

	inline void SetFrameCount(int frameCount)
	{
		m_BufferData.frameCount = frameCount;
	}

	inline void SetRowIndex(int rowIndex)
	{
		m_BufferData.rowIndex = rowIndex;
	}

	inline void SetChangeAnimation(bool change)
	{
		m_BufferData.changeAnimation = change;
	}

	inline void SetChangeRatio(float changeRatio)
	{
		m_BufferData.changeRatio = changeRatio;
	}

	inline void SetChangeFrameCount(int changeFrameCount)
	{
		m_BufferData.changeFrameCount = changeFrameCount;
	}
};

