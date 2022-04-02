#pragma once

#include "Hash.h"
#include "List.h"

template <typename Key, typename Value>
class CHashNode
{
	template <typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;

	template <typename Key, typename Value>
	friend class CHashTableIterator;

private:
	CHashNode()
	{
	}

	~CHashNode()
	{
		typename CList<CHashNode<Key, Value>*>::iterator	iter = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator	iterEnd = m_Chain.end();

		for (; iter != iterEnd; ++iter)
		{
			delete* iter;
		}
	}

public:
	Key		first;
	Value	second;

private:
	CList<CHashNode<Key, Value>*>	m_Chain;

private:
	void Add(const Key& key, const Value& value)
	{
		CHashNode<Key, Value>* Node = new CHashNode<Key, Value>;

		Node->first = key;
		Node->second = value;

		m_Chain.push_back(Node);
	}

	CHashNode<Key, Value>* Get(const Key& key)
	{
		typename CList<CHashNode<Key, Value>*>::iterator	iter = m_Chain.begin();
		typename CList<CHashNode<Key, Value>*>::iterator	iterEnd = m_Chain.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->first == key)
				return *iter;
		}

		return nullptr;
	}
};

template <typename Key, typename Value>
class CHashTableIterator
{
	template <typename Key, typename Value, int HASHSIZE>
	friend class CHashTable;

public:
	CHashTableIterator()
	{
		m_Node = nullptr;
	}

	~CHashTableIterator()
	{
	}

private:
	CHashNode<Key, Value>* m_Node;

public:
	bool operator == (const CHashTableIterator<Key, Value>& iter)	const
	{
		return m_Node == iter.m_Node;
	}

	bool operator != (const CHashTableIterator<Key, Value>& iter)	const
	{
		return m_Node != iter.m_Node;
	}

	CHashNode<Key, Value>* operator * ()
	{
		return m_Node;
	}
};


template <typename Key, typename Value, int HASHSIZE = 1223>
class CHashTable
{
public:
	CHashTable()
	{
	}

	~CHashTable()
	{
	}

private:
	typedef	CHashNode<Key, Value>	NODE;
	typedef	CHashNode<Key, Value>* PNODE;

public:
	typedef CHashTableIterator<Key, Value>	iterator;

private:
	NODE	m_Array[HASHSIZE];
	int		m_Size;

public:
	void insert(const Key& key, const Value& value)
	{
		CHash	hash;
		unsigned __int64	HashKey = hash.GetHash<Key>(key);

		int	Index = (int)(HashKey % HASHSIZE);

		m_Array[Index].Add(key, value);
		++m_Size;
	}

	int size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}

	Value& operator [] (const Key& key)
	{
		CHash	hash;
		unsigned __int64	HashKey = hash.GetHash<Key>(key);

		int	Index = (int)(HashKey % HASHSIZE);

		PNODE	Node = m_Array[Index].Get(key);

		if (!Node)
		{
			Value	addValue;
			m_Array[Index].Add(key, addValue);
			Node = m_Array[Index].Get(key);
		}

		return Node->second;
	}

	iterator Find(const Key& key)
	{
		CHash	hash;
		unsigned __int64	HashKey = hash.GetHash<Key>(key);

		int	Index = (int)(HashKey % HASHSIZE);

		PNODE	Node = m_Array[Index].Get(key);

		iterator	iter;
		iter.m_Node = Node;

		return iter;
	}

	bool IsValid(const iterator& iter)	const
	{
		return iter.m_Node != nullptr;
	}
};

