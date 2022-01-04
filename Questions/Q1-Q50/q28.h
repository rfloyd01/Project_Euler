#pragma once

#include <Header_Files/pch.h>

//Number spiral diagonals
std::pair<std::string, double> q28()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 1, current_num = 1;

	for (int i = 3; i <= 1001; i += 2)
	{
		for (int j = 0; j < 4; j++)
		{
			current_num += (i - 1);
			answer += current_num;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 669171001
	//ran in .0000067 seconds
}

//NOTES
//When looking at a 3x3 square you can see that each corner is only two numbers away from the corner before it. When looking at a
//5x5 square you can see that every corner is 4 numbers away from the corner before it, and so the pattern continues that in an 
//nxn square the corners will always be (n - 1) numbers away from eachother. In the above code, loop one designates the width of
//the square (which will always go up by two when created in the pattern the problem talks about) and loop two is from 1 to 4 to
//designate the numbers in each corner.
