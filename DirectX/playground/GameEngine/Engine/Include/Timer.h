#pragma once

#include "GameInfo.h"

class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER m_Time;
	LARGE_INTEGER m_Second;

	float m_DeltaTime;
	float m_CurrentTime;

	float m_FPS;
	float m_FPSTime;
	int m_Tick;

public:
	inline float GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	inline float GetFPS() const
	{
		return m_FPS;
	}

	inline float GetCurrentPlayTime() const
	{
		return m_CurrentTime;
	}

public:
	float Update();
};

