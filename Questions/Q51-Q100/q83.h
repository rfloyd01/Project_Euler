#pragma once

#include <Header_Files/pch.h>
#include <fstream>
#include <queue>

//Path Sum: Four Ways

void parseTextFile(int* board, int rows)
{
	std::ifstream myFile;
	myFile.open("Resources/q83.txt");

	std::string line;
	int num = 0, place;

	for (int i = 0; i < rows; i++)
	{
		std::getline(myFile, line);
		place = 0;
		for (int j = 0; j < line.size(); j++)
		{
			if (line[j] == ',')
			{
				board[rows * i + place] = num;
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
		board[rows * (i + 1) - 1] = num;
		num = 0;
	}
}

std::pair<std::string, double> q83()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	const int rows = 80, columns = 80;
	int board[rows * columns];
	bool visited[rows * columns] = { false };

	//Read the board in from the text file
	parseTextFile(board, rows);

	//Create a priority queue where each element represents a board tile (location and value). Initialize the queue
	//with the starting tile in the top left corner
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
	pq.push({ board[0], 0 });

	//Use Dijkstra's algorithm to find the fastest way to the bottom right corner of the board
	while (true)
	{
		//remove the element at the front of the priority queue
		std::pair<int, int> current_tile = pq.top();
		pq.pop();

		int current_row = current_tile.second / rows;
		int current_column = current_tile.second % columns;

		if (current_row == rows - 1 && current_column == columns - 1)
		{
			//we've reached the bottom right of the grid meaning our search is complete.
			//Set the answer and break out of the while loop
			answer = current_tile.first;
			break;
		}

		//mark the current tile as visited in the visited array
		visited[current_tile.second] = true;

		//check each of the 4 tiles surrounding the current one to see if anything should get added to the queue.
		//The order we check in is top, right, left and then bottom
		if (current_row > 0 && !visited[current_tile.second - columns]) pq.push({ current_tile.first + board[current_tile.second - columns], current_tile.second - columns });
		if (current_column < (columns - 1) && !visited[current_tile.second + 1]) pq.push({ current_tile.first + board[current_tile.second + 1], current_tile.second + 1 });
		if (current_column > 0 && !visited[current_tile.second - 1]) pq.push({ current_tile.first + board[current_tile.second - 1], current_tile.second - 1 });
		if (current_row < (rows - 1) && !visited[current_tile.second + columns]) pq.push({ current_tile.first + board[current_tile.second + columns], current_tile.second + columns });
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 425185
	//ran in 0.0022607 seconds
}

//NOTES
/*
* To solve this I decided to use Dijkstra's Algorithm. For the algorithm to work we need a priority queue, and a boolean matrix representing which locations we've 
* already found the minimal distance for.
* 
* The implementation ends up working perfectly, but, my priority queue class isn't optimized in the slightest. Some work to optimize this class would probably decrease 
* the run time by a large margin
*/

//UPDATE
/*
* The algorithm worked fine when I implemented my own hacky version of the Dijkstra algorithm, but I want to try again using built in data-structures as it'll probably
* be much faster. A quick glance at C++ priority queues and they don't actually look that simple to implement. I think what I'll do instead is use two separate arrays 
* to let us know which nodes have been visited already and which ones are still physically in the queue. Since we have an 80x80 grid there are 6,400 different tiles.
* Instead of using a row and column to define each tile we can just use a number from 0 to 6,399. To figure out the row and column from the single number all we need to 
* do is divide by 80 to get the row and modular divide by 80 to get the column. For example, the number 64 would be the (64 % 80) = 64th column of the floor(64, 80) = 0th row.
* Another example would be 6399 which would be the (6399 % 80) = 79th column of the floor(6399, 80) = 79th row.
* 
* Using single dimensional arrays will make things a little bit easier (although now that I think about it the added division might not be worth it).
* 
* After completing the algorithm using built ins the run time works out to be just about the same (actually a touch slower but only by ~1/10 millisecond). At the end of the day it 
* seems like making the swap wasn't necessary, although the code is a little more readable now which is nice.
* 
* One thing to note here is that the floor for runtime here would be around 0.0003 seconds as that's how long it takes to parse the text file with the board on it. The algorithms 
* I used in problems 81 and 82 have runtimes closer to this floor as they're simpler algorithms that don't have to take all four directions into account. Even if I could shave off 
* a little time here by using a different algorithm (like A*) I'm not really sure if it would be worth the effort.
*/