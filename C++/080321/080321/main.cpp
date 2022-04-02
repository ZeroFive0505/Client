#include <iostream>
#include <time.h>
#include "BinaryTree.h"

int Factorial(int Number)
{
	if (Number == 1)
		return 1;

	return Number * Factorial(Number - 1);
}

int FactorialTail(int Number, int Result)
{
	if (Number == 1)
		return Result;

	return FactorialTail(Number - 1, Result * Number);
}

int FactorialTail(int Number)
{
	return FactorialTail(Number, 1);
}

void Output(const int& Key, const int& Value)
{
	std::cout << "Key : " << Key << " Value : " << Value << "\n";
}

int main()
{
	srand((unsigned int)time(0));
	rand();
	//std::cout << FactorialTail(5) << std::endl;
	CBinaryTree<int, int>	tree;

	/*tree.insert(333, 333);

	for (int i = 0; i < 20; ++i)
	{
		int	Number = rand();
		tree.insert(Number, Number);
	}*/

	tree.insert(5, 5);
	tree.insert(3, 3);
	tree.insert(1, 1);
	tree.insert(4, 4);
	tree.insert(10, 10);
	tree.insert(8, 8);
	tree.insert(9, 9);
	tree.insert(15, 15);

	CBinaryTree<int, int>::iterator iter;


	for (iter = tree.begin(); iter != tree.end(); iter++)
	{
		std::cout << iter->first << " " << iter->second << "\n";
	}

	std::cout << "\n\n";

	tree.clear();

	for (iter = tree.begin(); iter != tree.end(); iter++)
	{
		std::cout << iter->first << " " << iter->second << "\n";
	}

	/*iter = tree.Find(512);

	if (iter == tree.end())
		std::cout << "Not found!\n";
	else
		std::cout << "Found!\n";*/

	std::cout << "============ PreOrder ============\n";
	tree.PreOrder(Output);
	std::cout << "\n";
	std::cout << "============ InOrder ============\n";
	tree.InOrder(Output);
	std::cout << "\n";
	std::cout << "============ PostOrder ============\n";
	tree.PostOrder(Output);
	std::cout << "\n";


	return 0;
}