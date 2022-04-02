#pragma once

#include "GameInfo.h"
/// <summary>
/// 타이머는 현재 몇 FPS인지와 전 프레임과 현재 프레임사이의 DeltaTime을 구하여
/// 오브젝트의 이동시에 곱해져서 부드러운 움직임과 컴퓨터의 성능과 상관없이 보장된 거리를 제공한다.
/// </summary>
class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_Time;
	LARGE_INTEGER	m_Second;

	float		m_DeltaTime;

	float		m_FPS;
	float		m_FPSTime;
	int			m_Tick;

public:
	float GetDeltaTime()	const
	{
		return m_DeltaTime;
	}

	float GetFPS()	const
	{
		return m_FPS;
	}

public:
	float Update();
};

