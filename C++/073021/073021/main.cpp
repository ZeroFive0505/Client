#include <iostream>
#include <time.h>
#include "Array.h"

bool SortInt(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	srand((unsigned int)time(0));
	CArray<int> Arr1;

	for (int i = 0; i < 10; i++)
	{
		Arr1.push_back(i + 1);
	}

	// 복사 생성자
	CArray<int> Arr2 = Arr1;

	CArray<int> Arr3;

	// 복사 대입 연산자
	Arr3 = Arr1;

	Arr1.erase(8);

	int Size = Arr1.size();
	for (int i = 0; i < Size; i++)
	{
		std::cout << Arr1[i] << "\n";
	}

	for (int i = 0; i < 100; i++)
		Arr3.push_back(rand() % 1000);

	Arr3.sort(SortInt);

	for (int i = 0; i < 100; i++)
	{
		std::cout << Arr3[i] << "\n";
	}

	return 0;
}