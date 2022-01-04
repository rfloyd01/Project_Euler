#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>
#include <fstream>

//Maximum path sum I
std::pair<std::string, double> q18()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;
	int number_of_rows = 15;

	std::vector<std::vector<int> > triangle;

	//first step is to copy the grid
	std::ifstream inFile;
	inFile.open("Resources/q18.txt");

	//the base of the triangle is only 15 numbers long, put the triangle into a vectorvector
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

	//instead of trying every possible path, you can start from the bottom left side of the triangle and compare each number to the number directly to it's right
	//whichever number is larger of the two, just add that number to the number inbetween them in the next row up of the triangle. The reasoning behind this is that if you knew the ideal path to
	//a certain point and only had two choices left, you would always choose the larger of the two numbers. So it only makes sense to pick the largest sums possible starting from the bottom

	//starting out on the second to last row and working upwards, change each number to be the sum of itself and the larger of the number below it or diagonally down and right from it

	/*
	Code to import triangle data left out for brevity
	*/

	for (int row = (number_of_rows - 2); row >= 0; row--)
	{
		for (int col = 0; col < triangle[row].size(); col++)
		{
			int larger_num = (triangle[row + 1][col] > triangle[row + 1][col + 1] ? triangle[row + 1][col] : triangle[row + 1][col + 1]);
			triangle[row][col] += larger_num;
		}
	}
	answer = triangle[0][0];
	//ran in 0.0002731 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 1074
	//ran in 0.0007187 seconds
}
