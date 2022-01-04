#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Counting Summations
std::pair<std::string, double> q76()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	int partitions[101] = { 0 };
	partitions[0] = 1;

	for (int i = 1; i < 101; i++)
		for (int j = i; j < 101; j++) partitions[j] += partitions[j - i];
	
	answer = partitions[100] - 1; //the minus 1 is because we need to remove the partition of 100 itself as it isn't the sum of two integers, just one
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 190569291
	//ran in 0.0000032 seconds
}

//NOTES
//This problem is almost identical to the coin counting problem (Project Euler question 31) with the caveat that instead of using coins with a value
//of only 1, 2, 5, 10, 20, 50, 100 or 200 we're using coins with a value of any number between 1 and 100. The total number of additions needed for this
//problem is 5050 so it runs basically instantly.