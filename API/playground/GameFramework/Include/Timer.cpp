#include "Timer.h"

CTimer::CTimer()	:
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0)
{
	// 고해상도 타이머의 초당 틱을 얻어온다.
	QueryPerformanceFrequency(&m_Second);

	// 고해상도 타이머의 현재 틱을 얻어온다.
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}
/// <summary>
/// 전과 현재 프레임에서 흐른 시간의 차이를 반환해준다.
/// </summary>
/// <returns>DeltaTime</returns>
float CTimer::Update()
{
	LARGE_INTEGER	Time;
	// 현재 시간이 얼마나 흘렀는지
	QueryPerformanceCounter(&Time);

	// 현재 흐른 시간과 멤버 변수로 가지고 있는 시간의 차이를 구하고
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	// 멤버 변수로 가지고있는 시간을 업데이트한다.
	m_Time = Time;

	// 흐른 시간만큼 FPS에 더한다.
	m_FPSTime += m_DeltaTime;
	// 1 프레임이 흘렀고
	++m_Tick;

	// 만약 60프레임일때
	if (m_Tick == 60)
	{
		// 초당 몇 프레임이 나왔는지 계산한다.
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}

	return m_DeltaTime;
}
