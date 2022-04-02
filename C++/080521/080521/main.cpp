#include <iostream>
#include "AVLTree.h"

void Output(const int& key, const int& value)
{
	std::cout << "Key : " << key << " Value : " << value << "\n";
}

int main()
{
	CAVLTree<int, int> tree;
	CAVLTree<int, int>::iterator iter;

	for (int i = 0; i < 10; i++)
	{
		tree.insert(i, i);
	}

	tree.erase(5);
	tree.erase(4);
	tree.erase(6);

	/*for (iter = tree.begin(); iter != tree.end(); iter++)
		std::cout << iter->first << ", " << iter->second << "\n";

	tree.clear();

	for (int i = 0; i < 10; i++)
	{
		tree.insert(i, i);
	}

	for (iter = tree.begin(); iter != tree.end(); iter++)
		std::cout << iter->first << ", " << iter->second << "\n";

	tree.InOrder(Output);
	
	std::cout << "======================================\n";

	tree.PostOrder(Output);

	std::cout << "======================================\n";

	tree.PreOrder(Output);*/


	for (iter = tree.begin(); iter != tree.end(); iter++)
	{
		std::cout << "Key : " << iter->first << " Value : " << iter->second << "\n";
		std::cout << "Parent Key : ";

		if (iter->IsParent())
			std::cout << iter->GetParentKey();
		else
			std::cout << "None";

		std::cout << " Parent Value : ";

		if (iter->IsParent())
			std::cout << iter->GetParentValue();
		else
			std::cout << "None";

		std::cout << "\n";

		std::cout << "Left Key : ";

		if (iter->IsLeft())
			std::cout << iter->GetLeftKey();
		else
			std::cout << "None";

		std::cout << " Left Value : ";

		if (iter->IsLeft())
			std::cout << iter->GetLeftValue();
		else
			std::cout << "None";

		std::cout << "\n";

		std::cout << "Right Key : ";

		if (iter->IsRight())
			std::cout << iter->GetRightKey();
		else
			std::cout << "None";

		std::cout << " Right Value : ";

		if (iter->IsRight())
			std::cout << iter->GetRightValue();
		else
			std::cout << "None";

		std::cout << "\n\n";
	}

	return 0;
}