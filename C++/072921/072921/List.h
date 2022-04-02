#pragma once

#include <assert.h>

// friend : 아래처럼 ListNode에 List를 friend로 지정하면 List에서는 ListNode의 private에 접근할 수 있다.

template <typename T>
class CListNode
{
	template <typename T>
	friend class CList;

	template <typename T>
	friend class CListIterator;

	template <typename T>
	friend class CListReverseIterator;

private:
	CListNode() : m_Next(nullptr), m_Prev(nullptr)
	{

	}

	~CListNode()
	{

	}

private:
	T m_Data;
	CListNode<T>* m_Next;
	CListNode<T>* m_Prev;
};

/*
* iterator : 노드를 순차적으로 방문하기 위해서 iterator를 만들어서 제공한다.
* iterator를 이용해서 앞에서부터 혹은 뒤에서부터 노드를 순차적으로 접근할 수 있게 해준다.
*/

template <typename T>
class CListIterator
{
	template<typename T>
	friend class CList;
public:
	CListIterator() : m_Node(nullptr)
	{

	}

	~CListIterator()
	{

	}

private:
	CListNode<T>* m_Node;

public:

	// iterator끼리 서로 가지고 있는 노드가 같을 경우 간다고 판단한다.
	bool operator==(const CListIterator<T>& iter) const
	{
		return m_Node == iter.m_Node;
	}

	bool operator ==(const CListNode<T>* Node) const
	{
		return m_Node == Node;
	}

	bool operator!=(const CListNode<T>* Node) const
	{
		return m_Node != Node;
	}


	bool operator!=(const CListIterator<T>& iter) const
	{
		return m_Node != iter.m_Node;
	}

	void operator++()
	{
		m_Node = m_Node->m_Next;
	}

	void operator++(int)
	{
		m_Node = m_Node->m_Next;
	}

	void operator--()
	{
		m_Node = m_Node->m_Prev;
	}

	void operator--(int)
	{
		m_Node = m_Node->m_Prev;
	}


	T& operator*() const
	{
		return m_Node->m_Data;
	}

};


template <typename T>
class CListReverseIterator
{
	template <typename T>
	friend class CList;
public:
	CListReverseIterator() : m_Node(nullptr)
	{

	}

	~CListReverseIterator()
	{

	}

private:
	CListNode<T>* m_Node;

public:
	bool operator==(const CListReverseIterator& iter) const
	{
		return this->m_Node == iter.m_Node;
	}

	bool operator!=(const CListReverseIterator& iter) const
	{
		return this->m_Node != iter.m_Node;
	}

	void operator++()
	{
		m_Node = m_Node->m_Prev;
	}

	void operator++(int)
	{
		m_Node = m_Node->m_Prev;
	}

	void operator--()
	{
		m_Node = m_Node->m_Next;
	}

	void operator--(int)
	{
		m_Node = m_Node->m_Next;
	}

	T& operator*() const
	{
		return m_Node->m_Data;
	}
};


template <typename T>
class CList
{
public:
	CList()
	{
		m_Size = 0;

		// Begin과 End노드를 생성하고 두 노드를 서로 연결한다.
		m_Begin = new NODE;
		m_End = new NODE;

		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}

	~CList()
	{
		// Begin노드는 이전 노드가 nullptr이다.
		// End노드는 다음 노드가 nullptr이다.

		PNODE DelteNode = m_Begin;

		while (DelteNode)
		{
			PNODE Next = DelteNode->m_Next;

			delete DelteNode;

			DelteNode = Next;
		}
	}

private:
	typedef CListNode<T> NODE;
	typedef CListNode<T>* PNODE;

public:
	typedef CListIterator<T> iterator;
	typedef CListReverseIterator<T> reverse_iterator;

private:
	// Begin과 End는 데이터를 저장하기 위한 노드는 아니다.
	// 명시적으로 시작과 끝을 의미하는 노드로 사용하기 위해 할당해두고 사용한다.
	// 실제 데이터를 저장하는 노드는 Begin과 End노드 사이에 위치하게 될 것이다.
	PNODE m_Begin;
	PNODE m_End;
	int m_Size;

public:
	void push_back(const T& data)
	{
		// 데이터를 저장해둘 노드를 생성한다.
		PNODE Node = new NODE;

		Node->m_Data = data;

		// End노드와 End노드의 이전 노드 사이에 새로 생성된 노드를 추가해주도록 한다.
		PNODE Prev = m_End->m_Prev;

		// End노드의 이전 노드의 다음 노드를 End에서 새로 생성된 노드로 교체한다.

		Prev->m_Next = Node;

		// 새로 생성된 노드의 이전 노드를 End의 이전 노드로 지정한다.
		Node->m_Prev = Prev;

		// 새로 생성된 노드의 다음 노드를 End노드로 지정한다.
		Node->m_Next = m_End;

		// End노드의 이전 노드를 새로 생성한 노드로 지정한다.
		m_End->m_Prev = Node;

		m_Size++;
	}

	void push_front(const T& data)
	{
		PNODE Node = new NODE;

		Node->m_Data = data;

		// Begin노드와 Begin노드의 다음 노드 사이에 새로 생성된 노드를 추가한다.
		PNODE Next = m_Begin->m_Next;

		// 새로 생성된 노드의 다음노드로 Begin노드의 다음 노드를 지정한다.
		Node->m_Next = Next;

		// Begin노드의 다음 노드의 이전 노드를 새로 생성된 노드로 지정한다.
		Next->m_Prev = Node;

		// Begin노드의 다음 노드로 새로 생성된 노드를 지정한다.
		m_Begin->m_Next = Node;

		// 새로 생성된 노드의 이전 노드로 Begin노드를 지정한다.
		Node->m_Prev = m_Begin;

		m_Size++;
	}


	// 마지막 노드를 제거한다.
	void pop_back()
	{
		if (empty())
		{
			assert(false);
		}

		// End노드의 이전 노드를 지워준다. 그러므로 End의 이전 노드를 얻어오고
		// End의 이전 노드의 이전 노드를 얻어와서 End와 연결해준다.
		PNODE DeleteNode = m_End->m_Prev;

		PNODE Prev = DeleteNode->m_Prev;

		// 지울 노드의 이전 노드의 다음 노드를 End노드로 지정한다.
		Prev->m_Next = m_End;

		// End노드의 이전 노드로 지울 노드의 이전 노드를 지정한다.
		m_End->m_Prev = Prev;

		delete DeleteNode;

		m_Size--;
	}

	void pop_front()
	{
		if (empty())
		{
			assert(false);
		}

		// Begin노드의 다음 노드를 지워준다.
		PNODE DeleteNode = m_Begin->m_Next;

		// 지울 노드의 다음 노드를 얻어온다.
		PNODE Next = DeleteNode->m_Next;

		// Begin노드의 다음 노드로 지울 노드의 다음 노드를 지정한다.
		m_Begin->m_Next = Next;

		// 지울 노드의 다음 노드의 이전 노드로 Begin노드를 지정한다.
		Next->m_Prev = m_Begin;

		// 노드를 제거한다.
		delete DeleteNode;

		m_Size--;
	}

	const T& front() const
	{
		if (empty())
		{
			assert(false);
		}

		return m_Begin->m_Next->m_Data;
	}

	const T& back() const
	{
		if (empty())
		{
			assert(false);
		}

		return m_End->m_Prev->m_Data;
	}


	// 클래스의 멤버함수는 함수의 뒤에 const를 붙일 수 있다.
	// 멤버함수 뒤에 const가 붙으면 이 함수에서는 멤버 변수의 값을 변경 불가능하다.
	// const 객체가 만들어진 경우라면 뒤에 const가 붙은 멤버만 호출가능하다.
	int size() const
	{
		return m_Size;
	}

	bool empty() const
	{
		return m_Size == 0;
	}

	// 추가된 모든 노드를 제거한다.
	void clear()
	{
		if (empty())
			return;

		PNODE DeleteNode = m_Begin->m_Next;

		// End노드가 아니라면 while을 돌면서 제거해주도록 한다.
		while (DeleteNode != m_End)
		{
			// 지울 노드의 다음 노드를 미리 받아둬야 한다.
			// 왜냐하면 노드를 먼저 지워버린다면 다음 노드를 얻어올 수 없다.
			PNODE Next = DeleteNode->m_Next;

			// 노드를 제거한다.
			delete DeleteNode;

			// 지울 노드를 다음 노드로 갱신한다.
			DeleteNode = Next;
		}

		// Begin노드와 End노드를 서로 연결해준다.
		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;

		// 노드의 갯수를 0으로 초기화.
		m_Size = 0;
	}

	iterator begin() const
	{
		iterator iter;
		iter.m_Node = m_Begin->m_Next;
		return iter;
	}

	iterator end() const
	{
		iterator iter;
		iter.m_Node = m_End;
		return iter;
	}

	reverse_iterator rbegin() const
	{
		reverse_iterator iter;
		iter.m_Node = m_End->m_Prev;
		return iter;
	}

	reverse_iterator rend() const
	{
		reverse_iterator iter;
		iter.m_Node = m_Begin;
		return iter;
	}

	void operator=(const CList<T>& list)
	{
		// 기존의 정보를 제거한다.
		clear();

		m_Size = list.m_Size;

		PNODE Node = list.m_Begin->m_Next;

		while (Node != list.m_End)
		{
			push_back(Node->m_Data);
			Node = Node->m_Next;
		}
	}


	// 삭제한 노드의 다음 노드를 가지고 있는 iterator를 반환해준다.
	iterator erase(const T& data)
	{
		iterator iter = Find(data);

		if (iter == end())
			return iter;

		return erase(iter);
	}

	iterator erase(const iterator& iter)
	{
		if (iter == end() || iter == m_Begin)
			return iter;

		// 지울 노드의 이전노드와 다음 노드를 얻어온다.
		// 지울 노드의 이전 노드와 다음 노드를 서로 연결시켜준다.
		PNODE Prev = iter.m_Node->m_Prev;
		PNODE Next = iter.m_Node->m_Next;

		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete iter.m_Node;

		m_Size--;

		iterator result;
		result.m_Node = Next;

		return result;
	}

	iterator Find(const T& data)
	{
		iterator iter = begin();
		iterator iterEnd = end();

		for (; iter != iterEnd; iter++)
		{
			if (*iter == data)
				return iter;
		}

		// 못찾을 경우 end를 리턴한다.
		return iterEnd;
	}

	// 정렬
	void sort(bool (*pFunc)(const T&, const T&))
	{
		iterator iter1 = begin();
		iterator iter1End = end();
		iter1End--;

		iterator iter2;
		iterator iter2End = end();

		// 예를 들어 10개라면 0 ~ 8까지만 반복한다.
		// 9번은 반복을 안한다.

		for (; iter1 != iter1End; iter1++)
		{
			iter2 = iter1;
			iter2++;
			for (; iter2 != iter2End; iter2++)
			{
				if (pFunc(*iter1, *iter2))
				{
					T data = *iter1;
					*iter1 = *iter2;
					*iter2 = data;
				}
			}
		}
	}
};
