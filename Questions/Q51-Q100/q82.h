#pragma once

#include <Header_Files/pch.h>
#include <fstream>
#include <sstream>
#include <string>

//Path Sum: Three Ways
std::pair<std::string, double> q82()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 427337; //trying to minimize answer, since board is same as last time but there are more moves start with a maximum of the answer to question 81

	const int rows = 80, columns = 80;
	int** board = new int* [rows];
	for (int i = 0; i < rows; i++) board[i] = new int[columns];

	//first save numbers from text file into an array
	std::ifstream myFile;
	myFile.open("Resources/q82.txt");

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
				board[i][place] = num;
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
		board[i][columns - 1] = num;
		num = 0;
	}

	int min, running_total;
	for (int j = columns - 2; j > 0; j--) //start on the second to last column and end on the second column
	{
		for (int i = rows - 1; i >= 0; i--) //check every row in the column starting at the bottom and moving to top
		{
			min = board[i][j] + board[i][j + 1]; //base minimum is always found by looking at box directly to the right
			if (i < (rows - 1)) //check downwards path if not on bottom row
				if (board[i][j] + board[i + 1][j] < min) min = board[i][j] + board[i + 1][j];

			//calculation of upwards path
			running_total = board[i][j];
			for (int k = i - 1; k >= 0; k--)
			{
				running_total += board[k][j];
				if (running_total > min)
				{
					break; //going up any higher will only exceed the current minimum by more, so break out
				}
				if (running_total + board[k][j + 1] < min) min = running_total + board[k][j + 1];
			}
			board[i][j] = min;
		}
	}
	
	for (int i = 0; i < rows; i++)
	{
		if (board[i][0] + board[i][1] < answer) answer = board[i][0] + board[i][1];
	}

	//clean up board array
	for (int i = 0; i < rows; i++) delete[] board[i];
	delete[] board;

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 260324
	//ran in 0.0006325 seconds
}

//NOTES
//As the problem states, this looks to be a harder version of problem 81 as there are now three directions that can be travelled in as opposed to two. Furthermore
//you can start from any box in the left most column, not just the top box. Luckily the text file is the exact same so none of the code has to be edited to get the
//appropriate numbers into an array. I think the best way to solve this problem is to start at the bottom of the column that's one away from the last column. See what
//the value would be to just go directly to the right and into the last column. From here go upwards, if the number directly above is less than the number directly to
//the right, look at the number directly to the right of the number directly above. If the sum of the number above and the number diagonally up and to the right is
//less then the number directly to the right, that is the new minimum. Keep going up the column until the number is greater than the current minimum. Now go up the 
//column to the next starting number. Look at the number directly to the right to get the base minimum. Then look to the number directly below to see if it's smaller
//than the base minimum, if so then that's the new minimum. Finally, follow the same procedure as before and keep advancing up the column and looking to the right
//until the current minimum is exceeded. Follow this procedure for every column from n-1 backwards to column 2. The answer will be the minimum value of the column 2
//numbers added to the numbers directly to their left in column 1 (as logically it wouldn't make sense to move upwards in column 1 as this will only increase the
//minimum value). Here's a visual representation of this algorithm using the last two columns from the sample text:
//
//  Start      last row    last row                           End
//              step 1      step 2
// 103  18 --> 103  18 --> 103  18 -----------------------> (103 + 18)        18
// 965 150 --> 965 150 --> 965 150 -----------------------> (965 + 103 + 18) 150 
// 422 111 --> 422 111 --> 422 111 -----------------------> (422 + 111)      111
// 121 956 --> 121 956 --> 121 956 -----------------------> (121 + 37 + 331) 956
//  37 331 --> 368 331 --> (37 + 121 + 956 < 368 ?) 331 --> (37 + 331)       331
//
//I skipped a few steps in there because it was a little tedious to write but it gets the idea across. For the last row it was most efficient to go straight to the right.
//For the next row up it made more sense to go down and pick up an extra box then to finish on the 956. Third row up was most efficient going to the right. The second row
//from the top actually made more sense to go upwards and to the right then to just go right so it goes to follow that for the top row going to the right was the best. Now
//the exact same algorithm is repeated for the next column over. We already know the most efficient way to get to the end from each spot in the second to last column so finding
//the most efficient way to get from the third to last column to the second to last column will effectively be the same as getting from the 3rd to last column to the last
//column.