#include "Timer.h"

CTimer::CTimer()	:
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0)
{
	// ���ػ� Ÿ�̸��� �ʴ� ƽ�� ���´�.
	QueryPerformanceFrequency(&m_Second);

	// ���ػ� Ÿ�̸��� ���� ƽ�� ���´�.
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}
/// <summary>
/// ���� ���� �����ӿ��� �帥 �ð��� ���̸� ��ȯ���ش�.
/// </summary>
/// <returns>DeltaTime</returns>
float CTimer::Update()
{
	LARGE_INTEGER	Time;
	// ���� �ð��� �󸶳� �귶����
	QueryPerformanceCounter(&Time);

	// ���� �帥 �ð��� ��� ������ ������ �ִ� �ð��� ���̸� ���ϰ�
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	// ��� ������ �������ִ� �ð��� ������Ʈ�Ѵ�.
	m_Time = Time;

	// �帥 �ð���ŭ FPS�� ���Ѵ�.
	m_FPSTime += m_DeltaTime;
	// 1 �������� �귶��
	++m_Tick;

	// ���� 60�������϶�
	if (m_Tick == 60)
	{
		// �ʴ� �� �������� ���Դ��� ����Ѵ�.
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}

	return m_DeltaTime;
}
