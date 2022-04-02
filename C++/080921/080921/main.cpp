#include <iostream>
#include "Graph.h"

void OutputNode(const int& data)
{
	std::cout << data << " -> ";
}

int main()
{

	CGraph<int> graph;

	for (int i = 1; i <= 10; i++)
		graph.insert(i);

	graph.AddEdge(1, 2);
	graph.AddEdge(2, 3);
	graph.AddEdge(3, 4);

	graph.AddEdge(1, 5);
	graph.AddEdge(5, 6);
	graph.AddEdge(6, 7);

	graph.AddEdge(1, 8);
	graph.AddEdge(8, 9);
	graph.AddEdge(9, 10);

	std::cout << "===================== BFS =====================\n";
	graph.BFS(OutputNode);
	std::cout << "\n";
	std::cout << "===================== DFS =====================\n";
	graph.DFS(OutputNode);
	std::cout << "\n";

	return 0;
}