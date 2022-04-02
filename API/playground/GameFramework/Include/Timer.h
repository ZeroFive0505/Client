#pragma once

#include "GameInfo.h"
/// <summary>
/// Ÿ�̸Ӵ� ���� �� FPS������ �� �����Ӱ� ���� �����ӻ����� DeltaTime�� ���Ͽ�
/// ������Ʈ�� �̵��ÿ� �������� �ε巯�� �����Ӱ� ��ǻ���� ���ɰ� ������� ����� �Ÿ��� �����Ѵ�.
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

