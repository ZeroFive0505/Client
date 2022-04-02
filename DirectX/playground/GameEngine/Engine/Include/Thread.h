
#pragma once

#include "GameInfo.h"

class CThread
{
protected:
	CThread();
	virtual ~CThread() = 0;

private:
	std::string m_Name;
	HANDLE m_Thread;
	HANDLE m_StartEvent;

protected:
	bool m_Loop;

public:
	inline void SetLoop(bool loop)
	{
		m_Loop = loop;
	}

public:
	bool Init();
	virtual void Run() = 0;

public:
	void Start();
	void Pause();
	void Resume();

private:
	void WaitStartEvent();
	// 멀티 스레드 함수 형태
	static unsigned int __stdcall ThreadFunction(void* arg);

public:
	template <typename T>
	static T* CreateThread(const std::string& name)
	{
		T* thread = new T;

		thread->m_Name = name;

		if (!thread->Init())
		{
			SAFE_DELETE(thread);
			return nullptr;
		}

		return thread;
	}
};