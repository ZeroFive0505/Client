#pragma once

#include <assert.h>

template <typename T>
class CQuickSort
{
public:
	CQuickSort()
	{
		m_Capacity = 8;
		m_Size = 0;

		m_Data = new T[m_Capacity];

		m_Func = SortFunction;
	}

	~CQuickSort()
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

		m_Data[m_Size++] = data;
	}

	void push(T* Array, int Count)
	{
		if (m_Capacity < Count)
		{
			m_Capacity = Count;
			delete[] m_Data;
			m_Data = new T[m_Capacity];
		}

		for (int i = 0; i < Count; i++)
		{
			m_Data[i] = Array[i];
		}

		m_Size = Count;
	}

	int size() const
	{
		return m_Size;
	}

	bool empty() const
	{
		return m_Size == 0;
	}

	void Sort()
	{
		QuickSort(0, m_Size - 1, m_Data);
	}

	void Sort(T* Array, int Count)
	{
		QuickSort(0, Count - 1, Array);
	}

private:
	void QuickSort(int Left, int Right, T* Array)
	{
		if (Left < Right)
		{
			int Pivot = Partition(Left, Right, Array);

			// 피봇 기준으로 왼쪽, 오른쪽을 분할해서 들어간다.
			QuickSort(Left, Pivot - 1, Array);
			QuickSort(Pivot + 1, Right, Array);
		}
	}

	int Partition(int Left, int Right, T* Array)
	{
		int Low = Left;
		int High = Right + 1;

		// 가장 왼쪽 값을 기준값으로 사용한다.
		T Value = Array[Left];

		do
		{
			do
			{
				Low++;
			} while (Low <= Right && m_Func(Value, Array[Low]));

			do
			{
				High--;
			} while (High >= Left && m_Func(Array[High], Value));

			if (Low < High)
			{
				T temp = Array[Low];
				Array[Low] = Array[High];
				Array[High] = temp;
			}

		} while (Low < High);

		T temp = Array[Left];
		Array[Left] = Array[High];
		Array[High] = temp;

		return High;
	}

private:
	static bool SortFunction(const T& Left, const T& Right)
	{
		return Left > Right;
	}
};

