#pragma once

#include <Header_Files/pch.h>

//Counting Block Combinations II
std::pair<std::string, double> q115()
{
	auto run_time = std::chrono::steady_clock::now();
	
	long long smallest_tile = 50, goal = 1000000;
	long long wall_size = smallest_tile + 1;
	std::vector<long long> ways(wall_size, 1);
	ways.back()++;

	while (ways.back() <= goal)
	{
		ways.push_back(1 + ways[wall_size - smallest_tile - 1] + ways[wall_size - 1]);
		ways[wall_size - smallest_tile] += ways[wall_size - smallest_tile - 1];
		wall_size++;

		//std::cout << ways.back() << std::endl;
	}

	long long answer = ways.size() - 1;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 168
	//ran in 0.0000056 seconds
}

//NOTES