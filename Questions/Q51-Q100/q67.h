#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>
#include <fstream>

//Maximum path sum II
std::pair<std::string, double> q67()
{
	//This is a harder version of question 18, the logic is the exact same though so this code is an exact copy other than the text file being imported and the number of rows of the triangle
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int number_of_rows = 100;

	std::vector<std::vector<int> > triangle;

	//first step is to copy the grid
	std::ifstream inFile;
	inFile.open("Resources/q67.txt");

	//the base of the triangle is 100 numbers long, put the triangle into a vectorvector
	for (int i = 0; i < number_of_rows; i++)
	{
		std::vector<int> row;
		for (int j = 0; j <= i; j++)
		{
			int num;

			inFile >> num;
			row.push_back(num);
		}
		triangle.push_back(row);
	}

	for (int row = (number_of_rows - 2); row >= 0; row--)
	{
		for (int col = 0; col < triangle[row].size(); col++)
		{
			int larger_num = (triangle[row + 1][col] > triangle[row + 1][col + 1] ? triangle[row + 1][col] : triangle[row + 1][col + 1]);
			triangle[row][col] += larger_num;
		}
	}

	answer = triangle[0][0];
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 7273
	//ran in 0.0077797 seconds
}