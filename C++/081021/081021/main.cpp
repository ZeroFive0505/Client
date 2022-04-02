
#include <iostream>
#include "Dijkstra.h"

int main()
{
	CDijkstra<const char*>	dijkstra;

	dijkstra.insert("1");
	dijkstra.insert("2");
	dijkstra.insert("3");
	dijkstra.insert("4");
	dijkstra.insert("5");
	dijkstra.insert("6");
	dijkstra.insert("7");

	dijkstra.AddEdge("1", "2", 10);
	dijkstra.AddEdge("1", "6", 5);

	dijkstra.AddEdge("2", "3", 5);
	dijkstra.AddEdge("2", "5", 9);

	dijkstra.AddEdge("3", "5", 6);
	dijkstra.AddEdge("3", "4", 4);

	dijkstra.AddEdge("4", "7", 7);

	dijkstra.AddEdge("5", "6", 7);
	dijkstra.AddEdge("5", "7", 6);

	CList<const char*>	FindList;

	if (dijkstra.Find(&FindList, "1", "4"))
	{
		CList<const char*>::iterator	iter;

		for (iter = FindList.begin(); iter != FindList.end(); ++iter)
		{
			std::cout << *iter << " -> ";
		}

		std::cout << std::endl;
	}

	else
	{
		std::cout << "길이 없다" << std::endl;
	}

	return 0;
}
