#pragma once

#include <Header_Files/pch.h>

std::pair<std::string, double> test()
{
	//ignore these lines, I use them in other header files
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	for (int i = 0; i < 100000000; i++)
	{
		int yeet = 0b0011001110;
		answer = log(yeet) / log(2);
	}
	

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in 0.245 seconds (this is current run time when goal is set to 10e14)
}
