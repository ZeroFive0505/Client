
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <algorithm>
#include "Heap.h"
#include "QuickSort.h"
#include "MergeSort.h"

bool Sort(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	srand((unsigned int)time(0));
	rand();

	//qsort()

	LARGE_INTEGER	PrevTick;
	LARGE_INTEGER	Tick;
	LARGE_INTEGER	SecondTick;

	QueryPerformanceFrequency(&SecondTick);

	CMergeSort<int>	Merge;
	Merge.SetSortFunction(Sort);

	int* Array = new int[100000];
	int* Array1 = new int[100000];
	int* Array2 = new int[100000];

	for (int i = 0; i < 100000; ++i)
	{
		Array[i] = rand();
		Array1[i] = Array[i];
		Array2[i] = Array[i];
		//std::cout << Array[i] << std::endl;
	}

	QueryPerformanceCounter(&PrevTick);

	Merge.Sort(Array, 100000);

	QueryPerformanceCounter(&Tick);

	float	Time = (Tick.QuadPart - PrevTick.QuadPart) / (float)SecondTick.QuadPart;

	std::cout << "Merge : " << Time << std::endl;

	std::cout << "============= Sort =============" << std::endl;
	/*for (int i = 0; i < 20000; ++i)
	{
		std::cout << Array[i] << std::endl;
	}*/


	CQuickSort<int>	Quick;
	Quick.SetSortFunction(Sort);

	//for (int i = 0; i < 200000; ++i)
	//{
	//	Array[i] = rand();
		//std::cout << Array[i] << std::endl;
	//}

	QueryPerformanceCounter(&PrevTick);

	Quick.Sort(Array1, 100000);

	QueryPerformanceCounter(&Tick);

	Time = (Tick.QuadPart - PrevTick.QuadPart) / (float)SecondTick.QuadPart;

	std::cout << "Quick : " << Time << std::endl;

	std::cout << "============= Sort =============" << std::endl;

	QueryPerformanceCounter(&PrevTick);

	for (int i = 0; i < 99999; ++i)
	{
		for (int j = i + 1; j < 100000; ++j)
		{
			if (Array2[i] > Array2[j])
			{
				int	Temp = Array2[i];
				Array2[i] = Array2[j];
				Array2[j] = Temp;
			}
		}
	}

	QueryPerformanceCounter(&Tick);

	Time = (Tick.QuadPart - PrevTick.QuadPart) / (float)SecondTick.QuadPart;

	std::cout << "Sort : " << Time << std::endl;

	/*for (int i = 0; i < 20000; ++i)
	{
		std::cout << Array[i] << std::endl;
	}*/

	/*CHeap<int>	heap;
	heap.SetSortFunction(Sort);

	for (int i = 0; i < 20; ++i)
	{
		heap.push(rand());
	}

	while (!heap.empty())
	{
		std::cout << heap.top() << std::endl;
		heap.pop();
	}*/

	delete[]	Array;
	delete[]	Array1;
	delete[]	Array2;

	return 0;
}
