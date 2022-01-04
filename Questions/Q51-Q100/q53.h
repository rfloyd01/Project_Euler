#pragma once

#include <Header_Files/pch.h>

//Combinatoric Selections
std::pair<std::string, double> q53()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int pascal_row[51] = { 0 }; //create an array which will be filled with the first half of each row of Pascal's Triangle, up to row 100
	pascal_row[0] = 1; //first number in each row is always 1
	int current_start = 1; //keeps track of which element in the row we will start addition from
	bool million_found = false; //once we've hit our first number over 1,000,000 we will use it to calculate where to start in the row

	for (int n = 2; n <= 100; n++)
	{
		for (int i = current_start; i > 0; i--)
		{
			if (pascal_row[i] == 0) pascal_row[i] = 2 * pascal_row[i - 1];
			else pascal_row[i] += pascal_row[i - 1];

			if (pascal_row[i] > 1000000)
			{
				million_found = true;
				current_start--;
			}
		}

		//since the million value has been breached, wherever current_start is located is the last number in the row below 1000000
	    //the number of numbers in the row greater than 1000000 therefore will be (n + 1) - 2 * (current_start + 1) which reduces to
		//n - 2 * current_start - 1.
		if (!million_found)
		{
			if (n % 2 == 1) current_start++;
		}
		else answer += (n - 2 * current_start - 1);
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4075
	//ran in .0000022 seconds
}

//NOTES
//After doing Project Euler for a little while my combinatorics skills have definitely improved, so at the start of this problem I can say I already know that
//if you find the answer to choose(n, r) for all r = 0-n, then you get the n-th row of Pascal's Triangle. Furthermore, choose(n, r) == choose(n, (n - r)) so
//the second half of each row in Pascal's Triangle is the same as the first. Instead of computing choose(n, r) for all n <= 100 we can just create the first half of each row
//of pascal's triangle using an iterative approach. Also, as we get further down in the triangle elements only increase so once a certain element in a row reaches 1000000, 
//we no longer need to keep adding onto it because it will continue to be greater than 1,000,000 as we go to the next row. Each row of the triangle has n + 1 elemnts in it
//which means row 100 should have 101 elemants, of which the first 51 will be distinct. Start off algorithm by creating an array of 51 elemnts. Then as we iterate n from 1 to 100.
//Go backwards through every element in the row and add the previous number to it, if n is odd then start just past the last element and set it equal to 2 times the current
//last element. Once we get our first element that's over a million we no longer need to add anything to it so start from an earlier element on the next row. The code ended up
//running basically instantaneously so I'm satisfied with how this turned out.