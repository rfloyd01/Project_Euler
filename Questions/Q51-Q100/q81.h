#pragma once

#include <Header_Files/pch.h>
#include <fstream>
#include <Header_Files/DataStructures_Algorithms/algorithms.h>

//Path Sum: Two Ways
std::pair<std::string, double> q81()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	const int rows = 80, columns = 80;
	/*int** board = new int* [rows];
	for (int i = 0; i < rows; i++) board[i] = new int[columns];*/

	//first save numbers from text file into an array
	std::ifstream myFile;
	myFile.open("Resources/q81.txt");

	std::string line;
	int num = 0, place;

	//Solve question using user defined Graph class and Dijkstra's algorithm
	std::vector<int> vertices;

	for (int i = 0; i < rows; i++)
	{
		std::getline(myFile, line);
		place = 0;
		for (int j = 0; j < line.size(); j++)
		{
			if (line[j] == ',')
			{
				//board[i][place] = num;
				vertices.push_back(num);
				num = 0;
				place++;
				continue;
			}
			else
			{
				num *= 10;
				num += (line[j] - 48);
			}
		}
		//board[i][columns - 1] = num;
		vertices.push_back(num);
		num = 0;
	}

	std::vector<std::pair<std::pair<int, int>, int>> edges;
	for (int row = 0; row < rows; row++)
	{
		//vertices all the way at the right of the graph will only have edges pointing downwards and
		//vertices at the bottome of the graph will only have edges pointing to the right. All other
		//vertices will have edges pointing to the right and downwards.
		int index = row * columns;
		for (int col = 0; col < columns; col++)
		{
			if ((index % columns) < (columns - 1)) edges.push_back({ {index, index + 1}, vertices[index + 1] });
			if (row < (rows - 1)) edges.push_back({ {index, index + columns}, vertices[index + columns] });
			index++;
		}
	}

	Graph<int> graph(vertices, edges, false);

	std::cout << DijkstrasAlgorithm(graph, *(*graph.first_vertex), *(*graph.last_vertex)) + vertices[0] << std::endl;

	//bottom row and right column should be added up first
	/*for (int i = rows - 2; i >= 0; i--)
	{
		board[rows - 1][i] += board[rows - 1][i + 1];
		board[i][columns - 1] += board[i + 1][columns - 1];
	}

	int min;
	for (int i = rows - 2; i >= 0; i--)
	{
		for (int j = columns - 2; j >= 0; j--)
		{
			min = (board[i + 1][j] < board[i][j + 1] ? board[i + 1][j] : board[i][j + 1]);
			board[i][j] += min;
		}
	}
	answer = board[0][0];*/

	//clean up board array
	/*for (int i = 0; i < rows; i++) delete[] board[i];
	delete[] board;*/

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 427337
	//ran in 0.0004875 seconds
}

//NOTES
//At it's heart this problem looks very similar to questions 18 and 67. Since we can only move in two directions, if you were to flip to array by 45 degrees so
//that it's a triangle it really would be the same other than the fact that everything will funnel to one point instead of multiple points. Start from the bottom
//right corner and add to it the smaller of the number directly below it and directly to the right (for numbers on the very bottom or right side you won't even
//have an option of what to add to it). The answer will be stored in the very first element of the array at the end of the loop.

//The hardest part of this problem ended up being reading the data from the text file into an array. I wasn't able to recognize the end of a line while trying to 
//read the data so I had to use the fact that no numbers above 9,999 were used in the matrix. If the numbers were completely random I'm not sure what I would've
//done outside of sticking a character on the end of each line in the original text file. Will need to look more into this.