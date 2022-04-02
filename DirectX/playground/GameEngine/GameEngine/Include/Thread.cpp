#include "Thread.h"

CThread::CThread() :
	m_Thread(0),
	m_StartEvent(0),
	m_Loop(false)
{
}

CThread::~CThread()
{
	// 소멸할때 락을 풀어준다.
	if (m_StartEvent)
	{
		CloseHandle(m_StartEvent);
		m_StartEvent = 0;
	}
}

bool CThread::Init()
{
	m_StartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	m_Thread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunction,
		this, 0, nullptr);

	return true;
}

void CThread::Start()
{
	SetEvent(m_StartEvent);
}

void CThread::Pause()
{
	DWORD count = 0;
	// 내부적으로 일시정지 카운트를 센다.
	// 모든 스레드를 일시정지하게 한다.
	do
	{
		count = SuspendThread(m_Thread);
	} while (count <= 0);
}

void CThread::Resume()
{
	DWORD count = 0;
	// 반대로 일시정지된 갯수만큼 반복하면서 스레드를
	// 활성화 시킨다.
	do
	{
		count = ResumeThread(m_Thread);
	} while (count >= 0);
}

void CThread::WaitStartEvent()
{
	WaitForSingleObject(m_StartEvent, INFINITE);
}

unsigned int __stdcall CThread::ThreadFunction(void* arg)
{
	CThread* thread = (CThread*)arg;

	thread->WaitStartEvent();

	thread->Run();

	return 0;
}
