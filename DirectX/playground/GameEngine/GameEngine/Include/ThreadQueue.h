#pragma once

#include "GameInfo.h"
#include "Sync.h"
#include <assert.h>

template <typename T, int SIZE = 200>
class CThreadQueue
{
private:
	T m_Queue[SIZE + 1];
	int m_Capacity;
	int m_Size;
	int m_Head;
	int m_Tail;
	CRITICAL_SECTION m_Crt;

public:
	CThreadQueue()
	{
		m_Size = 0;
		m_Head = 0;
		m_Tail = 0;

		m_Capacity = SIZE + 1;

		InitializeCriticalSection(&m_Crt);
	}

	~CThreadQueue()
	{
		DeleteCriticalSection(&m_Crt);
	}

public:
	void push(const T& data)
	{
		// 큐에 데이터를 넣을때에도 동기화를 시도해서 데이터를 보존한다.
		CSync sync(&m_Crt);

		int tail = (m_Tail + 1) % m_Capacity;

		// 꽉 찼을경우
		if (tail == m_Head)
			return;

		m_Queue[tail] = data;
		m_Tail = tail;

		m_Size++;
	}

	T& front()
	{	
		// 큐가 비었는지 확인하기전에 동기화를 시도하면안된다.
		// empty() 함수내에서도 동기화를 시도하므로 잘못될 경우 락이 제대로 안 풀릴 수가 있다.
		if (empty())
			assert(false);

		CSync sync(&m_Crt);

		int head = (m_Head + 1) % m_Capacity;

		return m_Queue[head];
	}

	void pop()
	{
		if (empty())
			assert(false);

		CSync sync(&m_Crt);

		m_Head = (m_Head + 1) % m_Capacity;

		m_Size--;
	}

	int size()
	{
		CSync sync(&m_Crt);

		return m_Size;
	}

	bool empty()
	{
		CSync sync(&m_Crt);

		return m_Size == 0;
	}

	void clear()
	{
		CSync sync(&m_Crt);

		m_Head = 0;
		m_Tail = 0;
		m_Size = 0;
	}
};

