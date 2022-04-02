#pragma once

#include <assert.h>

template <typename T>
class CMergeSort
{
public:
	CMergeSort()
	{
		m_Capacity = 8;
		m_Size = 0;

		m_Array = new T[m_Capacity];
		m_CopyArray = new T[m_Capacity];

		m_Func = SortFunction;
	}

	~CMergeSort()
	{
		delete[]	m_Array;
		delete[]	m_CopyArray;
	}

private:
	T* m_Array;
	T* m_CopyArray;
	int		m_Size;
	int		m_Capacity;
	bool(*m_Func)(const T&, const T&);

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

			memcpy(Array, m_Array, sizeof(T) * m_Size);

			delete[]	m_Array;
			delete[]	m_CopyArray;

			m_CopyArray = new T[m_Capacity];

			m_Array = Array;
		}

		m_Array[m_Size] = data;
		++m_Size;
	}

	void push(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[]	m_Array;
			delete[]	m_CopyArray;
			m_Array = new T[m_Capacity];
			m_CopyArray = new T[m_Capacity];
		}

		for (int i = 0; i < Count; ++i)
		{
			m_Array[i] = Array[i];
		}

		m_Size = Count;
	}

	int	size()	const
	{
		return m_Size;
	}

	bool empty()	const
	{
		return m_Size == 0;
	}

	void Sort()
	{
		MergeSort(0, m_Size - 1, m_Array);
	}

	void Sort(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[]	m_Array;
			delete[]	m_CopyArray;
			m_Array = new T[m_Capacity];
			m_CopyArray = new T[m_Capacity];
		}

		MergeSort(0, Count - 1, Array);
	}


private:
	void MergeSort(int Left, int Right, T* Array)
	{
		if (Left < Right)
		{
			// 가운데 인덱스를 구해준다.
			int	Mid = (Left + Right) / 2;

			// 왼쪽을 분할한다.
			MergeSort(Left, Mid, Array);

			// 오른쪽을 분할한다.
			MergeSort(Mid + 1, Right, Array);

			// 분할된 순서로 되돌아가며 정렬한다.
			Merge(Left, Mid, Right, Array);
		}
	}

	void Merge(int Left, int Mid, int Right, T* Array)
	{
		int	Low = Left;
		int	High = Mid + 1;
		int	Pivot = Left;

		while (Low <= Mid && High <= Right)
		{
			if (m_Func(Array[High], Array[Low]))
			{
				m_CopyArray[Pivot] = Array[Low];
				++Low;
				++Pivot;
			}

			else
			{
				m_CopyArray[Pivot] = Array[High];
				++High;
				++Pivot;
			}
		}

		// Low가 Mid보다 크다면 왼쪽배열의 값이 모두 처리가 되었다는 의미이다.
		if (Low > Mid)
		{
			for (int i = High; i <= Right; ++i)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;

			}
		}

		// 오른쪽 배열의 값이 모두 처리가 되었다는 의미이다.
		else
		{
			for (int i = Low; i <= Mid; ++i)
			{
				m_CopyArray[Pivot] = Array[i];
				++Pivot;
			}
		}

		// 복사본 배열에 정렬된 내용을 원본배열로 옮겨준다.
		for (int i = Left; i <= Right; ++i)
		{
			Array[i] = m_CopyArray[i];
		}
	}


private:
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};

