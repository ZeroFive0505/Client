#include <iostream>
#include "List.h"
#include <time.h>


/*
* �ڷᱸ�� : �����͸� �����ϴ� ���.
*/

// typedef :Ÿ���� �ٸ� �̸����� ������ ���ִ� ����� �����Ѵ�.
// typedef int MyInt;

bool SortInt(const int& Left, const int& Right)
{
	return Left > Right;
}



// F5�� ������ �ϸ� Debuging ���� ������ �ȴ�.
int main()
{
	srand((unsigned int)time(0));
	CList<int> ListInt;
	CList<float> ListFloat;


	for (int i = 0; i < 100; i++)
	{
		ListInt.push_back(i + 1);
	}

	CList<int>::iterator iter;

	iter = ListInt.erase(80);

	std::cout << "Next Node : " << *iter << "\n";

	ListInt.clear();

	for (int i = 0; i < 100; i++)
	{
		ListInt.push_back(rand() % 1000);
	}

	ListInt.sort(SortInt);


	for (iter = ListInt.begin(); iter != ListInt.end(); iter++)
		std::cout << *iter << " ";
	std::cout << "\n";

	/*for (iter = ListInt.begin(); iter != ListInt.end(); iter++)
		std::cout << *iter << " ";
	std::cout << "\n";*/

	/*for (iter = ListInt.rbegin(); iter != ListInt.rend(); iter++)
		std::cout << *iter << " ";
	std::cout << "\n";*/


	// const ��ü�� �Ϲ� ����Լ� ȣ���� �Ұ����ϴ�.
	// �Լ� �ڿ� cosnt�� �پ��ִ� �Լ��� ȣ���� �����ϴ�.
	// const CLIst<float> listFloat1;
	// listFloat1.push_back(10.1f);

	return 0;
}