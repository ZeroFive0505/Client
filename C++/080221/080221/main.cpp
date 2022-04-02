#include <iostream>
#include "Stack.h"
#include "Queue.h"
#include "CircleQueue.h"

/*
* Queue, Stack
* Stack : 선입 후출의 자료구조이다. 먼저 들어간 데이터가 나중에 나오는 구조이다.
* Queue:: 선입 선출의 자료구조이다. 먼저 들어간 데이터가 먼저 나오는 구조이다.
*/

int main()
{
	CCircleQueue<int, 10> Queue;

	for (int i = 0; i < 5; i++)
	{
		Queue.push(i);
	}

	for (int i = 0; i < 3; i++)
	{
		std::cout << Queue.front() << "\n";
		Queue.pop();
	}

	for (int i = 0; i < 10; i++)
	{
		Queue.push(i);
	}

	std::cout << "======\n";

	while (!Queue.empty())
	{
		std::cout << Queue.front() << "\n";
		Queue.pop();
	}

	/*
	CQueue<int> Queue;
	for (int i = 0; i < 100; i++)
	{
		Queue.push(i);
	}
	while (!Queue.empty())
	{
		std::cout << Queue.front() << "\n";
		Queue.pop();
	}*/

	/*int Array[100] = {};
	CStack<int> stackInt;
	for (int i = 0; i < 100; i++)
	{
		Array[i] = i + 1;
		stackInt.push(Array[i]);
	}
	for (int i = 0; i < 100; i++)
	{
		Array[i] = stackInt.top();
		stackInt.pop();
	}
	for (int i = 0; i < 100; i++)
	{
		std::cout << Array[i] << "\n";
	}
	while (!stackInt.empty())
	{
		std::cout << stackInt.top() << "\n";
		stackInt.pop();
	}*/

	return 0;
}