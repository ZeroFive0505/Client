#pragma once

#include <assert.h>

template <typename T>
class CQueueNode
{
	template <typename T>
	friend class CQueue;
private:
	CQueueNode() : m_Next(nullptr)
	{

	}

	~CQueueNode()
	{

	}

private:
	CQueueNode<T>* m_Next;
	T m_Data;
};


template <typename T>
class CQueue
{
public:
	CQueue()
	{
		m_FirstNode = nullptr;
		m_LastNode = nullptr;
		m_Size = 0;
	}

	~CQueue()
	{
		clear();
	}

private:
	CQueueNode<T>* m_FirstNode;
	CQueueNode<T>* m_LastNode;
	int m_Size;

public:
	void push(const T& data)
	{
		CQueueNode<T>* Node = new CQueueNode<T>;

		Node->m_Data = data;

		// 기존에 추가된 가장 마지막 노드의 다음 노드로 새로 생성된 노드를 지정한다.
		if (m_LastNode)
			m_LastNode->m_Next = Node;

		// 만약 처음 추가되는 노드라면 FirstNode를 새로 생성된 노드로 채워준다.
		if (!m_FirstNode)
			m_FirstNode = Node;

		// 가장 마지막 노드를 새로 생성된 노드로 갱신한다.
		m_LastNode = Node;

		m_Size++;
	}

	T& front() const
	{
		if (empty())
			assert(false);

		return m_FirstNode->m_Data;
	}

	void pop()
	{
		if (empty())
			assert(false);

		CQueueNode<T>* Next = m_FirstNode->m_Next;

		delete m_FirstNode;

		m_FirstNode = Next;

		if (!m_FirstNode)
			m_LastNode = nullptr;

		m_Size--;
	}

	int size() const
	{
		return m_Size;
	}

	bool empty() const
	{
		return m_Size == 0;
	}

	void clear()
	{
		while (m_FirstNode)
		{
			CQueueNode<T>* Next = m_FirstNode->m_Next;
			delete m_FirstNode;
			m_FirstNode = Next;
		}

		m_LastNode = nullptr;
		m_Size = 0;
	}
};