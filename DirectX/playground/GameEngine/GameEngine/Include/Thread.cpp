#include "Thread.h"

CThread::CThread() :
	m_Thread(0),
	m_StartEvent(0),
	m_Loop(false)
{
}

CThread::~CThread()
{
	// �Ҹ��Ҷ� ���� Ǯ���ش�.
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
	// ���������� �Ͻ����� ī��Ʈ�� ����.
	// ��� �����带 �Ͻ������ϰ� �Ѵ�.
	do
	{
		count = SuspendThread(m_Thread);
	} while (count <= 0);
}

void CThread::Resume()
{
	DWORD count = 0;
	// �ݴ�� �Ͻ������� ������ŭ �ݺ��ϸ鼭 �����带
	// Ȱ��ȭ ��Ų��.
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
