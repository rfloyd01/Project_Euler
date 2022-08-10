#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes NumberOfFactors()

//Highly divisible triangular number
std::pair<std::string, double> q12()
{
	//just used a brute force method for this one, should try and improve this at some point
	auto run_time = std::chrono::steady_clock::now();
	int triangle_num = 0;

	int num = 0;
	
	while (true)
	{
		num++;
		triangle_num += num;

		if (NumberOfFactors(triangle_num) > 500) break;
	}

	return { std::to_string(triangle_num), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 76576500
	//ran in 0.0992731 seconds
}

