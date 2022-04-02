#pragma once

template <typename KEY, typename VALUE>
class CBinaryTreeNode
{
	template <typename KEY, typename VALUE>
	friend class CBinaryTree;

	template <typename KEY, typename VALUE>
	friend class CBinaryTreeIterator;

private:
	CBinaryTreeNode()
		: m_Left(nullptr),
		m_Right(nullptr),
		m_Parent(nullptr),
		m_Next(nullptr),
		m_Prev(nullptr)
	{

	}

	~CBinaryTreeNode()
	{

	}

private:
	CBinaryTreeNode<KEY, VALUE>* m_Left;
	CBinaryTreeNode<KEY, VALUE>* m_Right;
	CBinaryTreeNode<KEY, VALUE>* m_Next;
	CBinaryTreeNode<KEY, VALUE>* m_Prev;
	CBinaryTreeNode<KEY, VALUE>* m_Parent;

public:
	KEY first;
	VALUE second;
};

template <typename KEY, typename VALUE>
class CBinaryTreeIterator
{
	template<typename KEY, typename VALUE>
	friend class CBinaryTree;

public:
	CBinaryTreeIterator() : m_Node(nullptr)
	{

	}

	~CBinaryTreeIterator()
	{

	}
private:
	CBinaryTreeNode<KEY, VALUE>* m_Node;

public:

	// iterator끼리 서로 가지고 있는 노드가 같을 경우 간다고 판단한다.
	bool operator==(const CBinaryTreeIterator<KEY, VALUE>& iter) const
	{
		return m_Node == iter.m_Node;
	}

	bool operator ==(const CBinaryTreeNode<KEY, VALUE>* Node) const
	{
		return m_Node == Node;
	}

	bool operator!=(const CBinaryTreeNode<KEY, VALUE>* Node) const
	{
		return m_Node != Node;
	}


	bool operator!=(const CBinaryTreeIterator<KEY, VALUE>& iter) const
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

	CBinaryTreeNode<KEY, VALUE>* operator->()
	{
		return m_Node;
	}
};

// Key는 탐색을 하기 위한 타입이다.
// Value는 실제 저장하기 위한 데이터 타입이다.
// 그래서 탐색을 할때는 Key타입으로 한다.
// 만약 Key타입이 문자열이라면 문자열로 탐색을 할 수 있는 것이다.
template <typename KEY, typename VALUE>
class CBinaryTree
{
public:
	CBinaryTree()
	{
		m_Root = nullptr;
		m_Size = 0;

		m_Begin = new NODE;
		m_End = new NODE;

		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;
	}

	~CBinaryTree()
	{
		PNODE DeleteNode = m_Begin;

		while (DeleteNode)
		{
			PNODE Next = DeleteNode->m_Next;
			delete DeleteNode;

			DeleteNode = Next;
		}
	}

private:
	typedef CBinaryTreeNode<KEY, VALUE> NODE;
	typedef CBinaryTreeNode<KEY, VALUE>* PNODE;

private:
	PNODE m_Root;
	PNODE m_Begin;
	PNODE m_End;
	int m_Size;

public:
	typedef CBinaryTreeIterator<KEY, VALUE> iterator;

public:
	void insert(const KEY& key, const VALUE& value)
	{
		// 처음 데이터를 추가할 경우
		if (!m_Root)
		{
			m_Root = new NODE;

			m_Root->first = key;
			m_Root->second = value;

			m_Begin->m_Next = m_Root;
			m_Root->m_Prev = m_Begin;

			m_Root->m_Next = m_End;
			m_End->m_Prev = m_Root;
		}
		else
		{
			insert(key, value, m_Root);
		}

		m_Size++;
	}

	int size() const
	{
		return m_Size;
	}

	bool empty() const
	{
		return m_Size == 0;
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

	iterator Find(const KEY& key)
	{
		return Find(key, m_Root);
	}

	void clear()
	{
		PNODE Node = m_Begin->m_Next;

		while (Node != m_End)
		{
			PNODE Next = Node->m_Next;

			delete Node;

			Node = Next;
		}

		m_Begin->m_Next = m_End;
		m_End->m_Prev = m_Begin;

		m_Size = 0;

		m_Root = nullptr;
	}

	/*
	* 이진트리는 3가지의 순회 방법이 있다.
	* 전위순회, 중위순회, 후위순회
	* 전위순회 : Root -> Left -> Right
	* 중위순회 : Left -> Root -> Right
	* 후위순회 : Left -> Right -> Root
	*/

	void PreOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		PreOrder(pFunc, m_Root);
	}

	void InOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		InOrder(pFunc, m_Root);
	}

	void PostOrder(void(*pFunc)(const KEY&, const VALUE&))
	{
		PostOrder(pFunc, m_Root);
	}

	iterator erase(const KEY& key)
	{
		iterator iter = Find(key);

		if (iter == end())
			return iter;

		return erase(iter);
	}

	iterator erase(const iterator& iter)
	{
		if (iter == end())
			return iter;
		else if (iter == m_Begin)
			return end();
		else if (iter.m_Node == nullptr)
			return end();

		// 리프 노드일 경우 부모로부터 연결을 제거하고 노드를 제거해준다.
		if (!iter.m_Node->m_Left && !iter.m_Node->m_Right)
		{
			// 부모노드를 얻어온다.
			PNODE Parent = iter.m_Node->m_Parent;

			// 만약 부모노드가 없다면 현재 지우려는 노드는 루드 노드라는 것이다.
			// 그런데 루트노드가 리프노드라는 말은 이 노드 1개만 남았다는 것이다.
			if (!Parent)
			{
				delete iter.m_Node;

				m_Root = nullptr;
				m_Size--;

				// 다 지웠으니 Begin과 End끼리 연결한다.
				m_Begin->m_Next = m_End;
				m_End->m_Prev = m_Begin;

				return end();
			}

			// 지우려는 노드가 부모노드의 왼쪽 노드인지 오른쪽 노드인지를 파악하여 부모와의 연결을 끊어준다.
			if (Parent->m_Left == iter.m_Node)
				Parent->m_Left = nullptr;
			else
				Parent->m_Right = nullptr;

			PNODE Prev = iter.m_Node->m_Prev;
			PNODE Next = iter.m_Node->m_Next;

			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			delete iter.m_Node;

			m_Size--;


			// 지운 노드의 다음 노드를 넣어서 반환해준다.
			iterator result;

			result.m_Node = Next;

			return result;
		}

		// 만약 왼쪽 노드가 있을 경우 왼쪽 서브 트리에서 가장 큰 노드를 찾아서
		// 지울 노드의 값을 변경해주고 찾아준 노드를 제거한다.
		if (iter.m_Node->m_Left)
		{
			// 왼쪽 서브트리에 가장 큰 값을 가지고 있는 노드를 찾아온다.
			PNODE MaxNode = FindMax(iter.m_Node->m_Left);

			// 찾아준 노드의 Key, Value 값으로 변경해준다.
			iter.m_Node->first = MaxNode->first;
			iter.m_Node->second = MaxNode->second;

			// 찾아준 노드를 제거해야하기 때문에 부모로부터 연결을 끊고
			// 제거해주도록한다.
			// 단, 찾아준 노드가 왼쪽 자식노드가 있을수도 있으므로
			PNODE LeftChild = MaxNode->m_Left;
			PNODE Parent = MaxNode->m_Parent;


			// 왼쪽에 바로 붙어있을 경우. 즉 자식이 하나만 있으며 이 자식이 왼쪽으로 쭉 연결되있을 경우
			if (Parent->m_Left == MaxNode)
				Parent->m_Left = LeftChild;
			else
				Parent->m_Right = LeftChild;

			// 왼쪽 자식노드가 있을 경우라면 부모로 MaxNode의 부모를 지정해주도록 한다.
			if (LeftChild)
				LeftChild->m_Parent = Parent;

			PNODE Prev = MaxNode->m_Prev;
			PNODE Next = MaxNode->m_Next;

			Prev->m_Next = Next;
			Next->m_Prev = Prev;

			delete MaxNode;

			m_Size--;

			iterator result;
			result.m_Node = Next;

			return result;
		}

		// 지울 노드의 오른쪽 노드만 존재할 경우 오른쪽 노드에서 가장 적은 노드를 찾아온다.
		PNODE MinNode = FindMin(iter.m_Node->m_Right);

		// 찾아준 노드의 Key, Value 값으로 변경해준다.
		iter.m_Node->first = MinNode->first;
		iter.m_Node->second = MinNode->second;

		// 찾아준 노드를 제거해야하기 때문에 부모로부터 연결을 끊고
		// 제거해주도록한다.
		// 단, 찾아준 노드가 오른쪽 자식노드가 있을수도 있으므로
		PNODE RightChild = MinNode->m_Left;
		PNODE Parent = MinNode->m_Parent;

		if (Parent->m_Left == MinNode)
			Parent->m_Left = RightChild;
		else
			Parent->m_Right = RightChild;

		// 왼쪽 자식노드가 있을 경우라면 부모로 MaxNode의 부모를 지정해주도록 한다.
		if (RightChild)
			RightChild->m_Parent = Parent;

		PNODE Prev = MinNode->m_Prev;
		PNODE Next = MinNode->m_Next;

		Prev->m_Next = Next;
		Next->m_Prev = Prev;

		delete MinNode;

		m_Size--;

		iterator result;
		result.m_Node = Next;

		return result;
	}

private:
	PNODE insert(const KEY& key, const VALUE& value, PNODE Node)
	{
		// 기준노드보다 작다면 왼쪽이다.
		if (Node->first > key)
		{
			// 만약 기준노드의 왼쪽 자식 노드가 있다면 그 왼쪽 자식노드를 기준노드로 하여 다시 탐색을 하게한다.
			if (Node->m_Left)
				return insert(key, value, Node->m_Left);
			// 더 이상 왼쪽 자식 노드가 없을 경우 이 위치에 새로 노드를 생성하여 추가한다.
			PNODE NewNode = new NODE;

			NewNode->first = key;
			NewNode->second = value;

			// 기준노드의 왼쪽 자식노드로 지정한다.
			Node->m_Left = NewNode;
			NewNode->m_Parent = Node;


			// 왼쪽으로 배치가 된다는 것은 부모노드보다 작다는 것이다.
			// 그러므로 부모노드의 이전노드와 부모노드 사이에 새로 생성된 노드를 리스트로 연결한다.
			PNODE Prev = Node->m_Prev;

			Prev->m_Next = NewNode;
			NewNode->m_Prev = Prev;

			NewNode->m_Next = Node;
			Node->m_Prev = NewNode;

			return NewNode;
		}



		// 여기로 내려오면 값이 크다는 것이므로 오른쪽을 탐색 해봐야 한다.
		// 만약 오른쪽 자식노드가 있을 경우 기준노드를 오른쪽 자식노드로 하여 탐색하게한다.
		if (Node->m_Right)
			return insert(key, value, Node->m_Right);

		PNODE NewNode = new NODE;

		NewNode->first = key;
		NewNode->second = value;

		// 기준노드의 오른쪽 자식노드로 지정한다.
		Node->m_Right = NewNode;
		NewNode->m_Parent = Node;

		PNODE Next = Node->m_Next;

		Node->m_Next = NewNode;
		NewNode->m_Prev = Node;

		NewNode->m_Next = Next;
		Next->m_Prev = NewNode;

		return NewNode;
	}

	iterator Find(const KEY& Key, PNODE Node) const
	{
		// 기준노드가 nullptr일 경우 더 이상 탐색할 노드가 없으므로
		// end를 리턴한다. 못찾았다는 것이다.
		if (!Node)
			return end();

		if (Node->first == Key)
		{
			iterator iter;
			iter.m_Node = Node;

			return iter;
		}

		// 키를 비교하여 작으면 왼쪽 크면 오른쪽으로 탐색해서 들어간다.
		if (Node->first > Key)
			return Find(Key, Node->m_Left);

		return Find(Key, Node->m_Right);
	}


	void PreOrder(void(*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		pFunc(Node->first, Node->second);
		PreOrder(pFunc, Node->m_Left);
		PreOrder(pFunc, Node->m_Right);
	}

	void InOrder(void(*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		InOrder(pFunc, Node->m_Left);
		pFunc(Node->first, Node->second);
		InOrder(pFunc, Node->m_Right);
	}

	void PostOrder(void(*pFunc)(const KEY&, const VALUE&), PNODE Node)
	{
		if (!Node)
			return;

		PostOrder(pFunc, Node->m_Left);
		PostOrder(pFunc, Node->m_Right);
		pFunc(Node->first, Node->second);
	}

	PNODE FindMax(PNODE Node)
	{
		if (Node->m_Right)
			return FindMax(Node->m_Right);

		return Node;
	}

	PNODE FindMin(PNODE Node)
	{
		if (Node->m_Left)
			return FindMin(Node->m_Left);

		return Node;
	}
};