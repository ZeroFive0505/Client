#pragma once

#include "Thread.h"
#include "ThreadQueue.h"
#include "../Client.h"

class CLoadingThread :
	public CThread
{
public:
	CLoadingThread();
	virtual ~CLoadingThread();

private:
	CThreadQueue<sLoadingMessage> m_Queue;

public:
	inline CThreadQueue<sLoadingMessage>* GetLoadingQueue()
	{
		return &m_Queue;
	}

	inline void AddMessage(bool complete, float percent)
	{
		sLoadingMessage msg;

		msg.complete = complete;
		msg.percent = percent;
		m_Queue.push(msg);
	}

public:
	bool Init();
	virtual void Run();
};

