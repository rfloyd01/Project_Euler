#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h>
#include <Header_Files/DataStructures_Algorithms/algorithms.h>
#include <Header_Files/DataStructures_Algorithms/datastructures.h>
#include <Header_Files/DataStructures_Algorithms/graph.h>

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	//Graph Debugging
	std::vector<char> vertices = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I' };
	std::vector<std::pair<std::pair<int, int>, int>> edges = { {{0, 1}, 4}, {{1, 2}, 8}, {{2, 3}, 7}, {{3, 4}, 9}, {{4, 5}, 10}, {{5, 6}, 2},
	{{6, 7}, 1}, {{7, 8}, 7}, {{0, 7}, 8}, {{7, 1}, 11}, {{8, 2}, 2}, {{8, 6}, 6}, {{5, 2}, 4}, {{3, 5}, 14} };

	Graph<char> graph(vertices, edges);
	//graph.print();

	//DepthFirstSearch(graph, *(*graph.getVertices()->begin()));
	//BreadthFirstSearch(graph, *(*graph.getVertices()->begin()));

	auto start = graph.getVertices()->begin(), end = graph.getVertices()->begin();
	for (auto it = graph.getVertices()->begin(); it != graph.getVertices()->end(); it++)
	{
		if ((*it)->getData() == 'A') start = it;
		else if ((*it)->getData() == 'E') end = it;
	}
	
	std::cout << DijkstrasAlgorithm(graph, *(*start), *(*end)) << std::endl;

	//pq testing
	/*std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;
	std::pair<int, std::string> t1 = { 1, "Hello"}, t2 = {1, "AWorld"}, t3 = {3, "Hello" };

	pq.push(t1); pq.push(t2); pq.push(t3);
	while (!pq.empty())
	{
		std::cout << pq.top().first << ", " << pq.top().second << std::endl;
		pq.pop();
	}

	std::cout << std::endl;*/
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}