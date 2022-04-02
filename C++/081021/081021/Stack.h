#pragma once

#include <assert.h>

template <typename T>
class CStackNode
{
	template <typename T>
	friend class CStack;

private:
	CStackNode()
		: m_Next(nullptr)
	{

	}

	~CStackNode()
	{

	}

private:
	CStackNode<T>* m_Next;
	T m_Data;
};

template <typename T>
class CStack
{
public:
	CStack()
	{
		m_LastNode = nullptr;
		m_Size = 0;
	}

	~CStack()
	{
		clear();
	}

private:
	// 가장 마지막에 추가된 노드의 주소를 담는다.
	CStackNode<T>* m_LastNode;
	int m_Size;

public:
	void push(const T& data)
	{
		// 데이터를 저장하기 위한 노드를 생성한다.
		CStackNode<T>* Node = new CStackNode<T>;

		Node->m_Data = data;

		// 새로 생성된 노드의 다음 노드를 기존에 마지막에 추가된 노드로 지정해준다.
		// 만약 처음 추가하는 노드라면 m_LastNode는 nullptr이 들어가 있으므로
		// 새로 생성된 노드의 다음으로는 nullptr이 지정될 것이며
		// 즉, 다음 노드가 nullptr이라면 더 이상 노드가 없다는 의미이다.
		Node->m_Next = m_LastNode;

		// 마지막으로 추가된 노드를 갱신해준다.
		m_LastNode = Node;

		m_Size++;
	}

	T& top() const
	{
		if (empty())
			assert(false);

		return m_LastNode->m_Data;
	}

	void pop()
	{
		if (empty())
			assert(false);

		CStackNode<T>* Next = m_LastNode->m_Next;

		delete m_LastNode;

		m_LastNode = Next;

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
		CStackNode<T>* Node = m_LastNode;

		while (m_LastNode)
		{
			CStackNode<T>* Next = m_LastNode->m_Next;
			delete m_LastNode;
			m_LastNode = Next;
		}

		m_Size = 0;
	}
};
