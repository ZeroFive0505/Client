#include <iostream>
#include "List.h"
#include <time.h>


/*
* 자료구조 : 데이터를 관리하는 방법.
*/

// typedef :타입을 다른 이름으로 재정의 해주는 기능을 제공한다.
// typedef int MyInt;

bool SortInt(const int& Left, const int& Right)
{
	return Left > Right;
}



// F5로 실행을 하면 Debuging 모드로 실행이 된다.
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


	// const 객체는 일반 멤버함수 호출이 불가능하다.
	// 함수 뒤에 cosnt가 붙어있는 함수만 호출이 가능하다.
	// const CLIst<float> listFloat1;
	// listFloat1.push_back(10.1f);

	return 0;
}