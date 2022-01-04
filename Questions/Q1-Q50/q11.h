#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>
#include <fstream>

//Largest product in a grid
std::pair<std::string, double> q11()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0;
	std::vector<std::vector<int> > grid;

	//first step is to copy the grid
	std::ifstream inFile;
	inFile.open("C:/Users/Bobby/Documents/Coding/C++/Project_Euler/Resources/q11.txt");

	for (int i = 0; i < 20; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < 20; j++)
		{
			int num;
			inFile >> num;
			row.push_back(num);
		}
		grid.push_back(row);
	}
	
	for (int row = 0; row < grid.size(); row++)
	{
		for (int col = 0; col < grid[row].size(); col++)
		{
			bool vert_good = 0, horz_good = 0;
			if (col <= grid[row].size() - 4) //check all horizontal options
			{
				horz_good = 1;
				int horizontal = grid[row][col] * grid[row][col + 1] * grid[row][col + 2] * grid[row][col + 3];
				if (horizontal > answer) answer = horizontal;
			}

			if (row <= grid.size() - 4) //check all vertical options
			{
				vert_good = 1;
				int vertical = grid[row][col] * grid[row + 1][col] * grid[row + 2][col] * grid[row + 3][col];
				if (vertical > answer) answer = vertical;
			}

			if (horz_good & vert_good) //check all diagonal down right options
			{
				int diag_right = grid[row][col] * grid[row + 1][col + 1] * grid[row + 2][col + 2] * grid[row + 3][col + 3];
				if (diag_right > answer) answer = diag_right;
			}

			if ((col >= 3) & vert_good) //check all diagonal down left options
			{
				int diag_left = grid[row][col] * grid[row + 1][col - 1] * grid[row + 2][col - 2] * grid[row + 3][col - 3];
				if (diag_left > answer) answer = diag_left;
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 70600674
	//ran in 0.0003631 seconds
}
