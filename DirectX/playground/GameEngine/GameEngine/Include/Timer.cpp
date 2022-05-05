#include "Timer.h"
#include "Engine.h"

CTimer::CTimer() :
    m_FPS(0.0f),
    m_FPSTime(0.0f),
    m_Tick(0)
{
    QueryPerformanceFrequency(&m_Second);
    m_CurrentTime = 0.0f;
    QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{
}

float CTimer::Update()
{
    LARGE_INTEGER Time;

    QueryPerformanceCounter(&Time);

    m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

    m_Time = Time;

    m_FPSTime += m_DeltaTime;
    m_Tick++;

    if (m_Tick == 60)
    {
        m_FPS = 60 / m_FPSTime;
        m_FPSTime = 0.0f;
        m_Tick = 0;

        char fps[26] = {};

        sprintf_s(fps, "FPS : %.5f", m_FPS);
        SetWindowTextA(CEngine::GetInst()->GetWindowHandle(), fps);
    }

    m_CurrentTime += m_DeltaTime;

    return m_DeltaTime;
}
