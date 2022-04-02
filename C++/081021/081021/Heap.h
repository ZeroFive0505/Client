#pragma once

#include <string.h>
#include <assert.h>

template <typename T>
class CHeap
{
public:
	CHeap()
	{
		m_Capacity = 8;
		m_Size = 0;

		m_Data = new T[m_Capacity];

		m_Func = SortFunction;
	}

	~CHeap()
	{
		delete[] m_Data;
	}

private:
	T* m_Data;
	int m_Size;
	int m_Capacity;
	bool (*m_Func)(const T&, const T&);

public:
	void SetSortFunction(bool (*pFunc)(const T&, const T&))
	{
		m_Func = pFunc;
	}

public:
	void push(const T& data)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* Array = new T[m_Capacity];

			memcpy(Array, m_Data, sizeof(T) * m_Size);

			delete[] m_Data;

			m_Data = Array;
		}

		m_Data[m_Size] = data;

		// 힙으로 구성한다.
		InsertHeap(m_Size);


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

	T top() const
	{
		if (empty())
			assert(false);

		return m_Data[0];
	}

	void clear()
	{
		m_Size = 0;
	}

	void pop()
	{
		if (empty())
			assert(false);
		else if (m_Size == 1)
		{
			m_Size = 0;
			return;
		}

		m_Size--;

		// 가장 마지막에 추가된 노드를 루트로 올린다.
		m_Data[0] = m_Data[m_Size];
		DeleteHeap(0);
	}

private:
	void InsertHeap(int Index)
	{
		// Index가 0이면 루트노드라는 의미이다. 그러므로 더 이상 작업을 안 하고 반환한다.
		if (Index == 0)
			return;

		// 부모노드의 인덱스를 구해준다.
		int ParentIndex = (Index - 1) / 2;

		if (m_Func(m_Data[ParentIndex], m_Data[Index]))
		{
			T temp = m_Data[ParentIndex];
			m_Data[ParentIndex] = m_Data[Index];
			m_Data[Index] = temp;

			InsertHeap(ParentIndex);
		}
	}

	void DeleteHeap(int Index)
	{
		// 왼쪽 자식노드의 인덱스를 구해준다.
		int LeftChildIndex = Index * 2 + 1;

		// m_Size보다 크거나 같다면 더 이상 자식노드가 없다는 의미이다.
		if (LeftChildIndex >= m_Size)
			return;

		// 오른쪽 자식노드의 인덱스를 구한다.
		int RightChildIndex = LeftChildIndex + 1;

		int ChildIndex = LeftChildIndex;

		// 오른쪽 자식노드가 있는지 판단한다.
		if (RightChildIndex < m_Size)
		{
			// 오른쪽 자식노드가 있을 경우 왼쪽과 오른쪽 값을 비교하여 선택해준다.
			if (m_Func(m_Data[LeftChildIndex], m_Data[RightChildIndex]))
				ChildIndex = RightChildIndex;
		}

		// 최종 선택된 ChildIndex와 값을 비교한다.
		if (m_Func(m_Data[Index], m_Data[ChildIndex]))
		{
			T temp = m_Data[Index];
			m_Data[Index] = m_Data[ChildIndex];
			m_Data[ChildIndex] = temp;

			DeleteHeap(ChildIndex);
		}
	}

private:
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};

