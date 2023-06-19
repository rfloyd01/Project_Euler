#pragma once

#include <Header_Files/pch.h>

//Red, Green, and Blue Tiles
std::pair<std::string, double> q117()
{
	auto run_time = std::chrono::steady_clock::now();
	
	long long ways[51] = { 1, 1, 2, 4 };

	for (int i = 4; i <= 50; i++)
		ways[i] = ways[i - 2] + ways[i - 3] + ways[i - 4] + ways[i - 1];

	long long answer = ways[7];
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 100808458960497
	//ran in 0.0000009 seconds
}

//NOTES