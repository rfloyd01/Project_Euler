#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <vector>

//Non-abundant sums
std::pair<std::string, double> q23()
{
	auto run_time = std::chrono::steady_clock::now();

	int answer = 1, maximum = 28123; //set answer to 1 because most loops below start at 2 and don't want 1 to get left out by mistake
	std::vector<int> divisor_sums, abundant_numbers;
	for (int i = 0; i < maximum; i++) divisor_sums.push_back(1); //start with 1 because every number is divisible by one

	//use a sieve to get the divisor sums for every number under 28,123
	for (int i = 2; i < maximum; i++)
	{
		if (divisor_sums[i] > i) abundant_numbers.push_back(i);
		divisor_sums[i] = 0;
		for (int j = i + i; j < maximum; j += i) divisor_sums[j] += i;
	}

	//this loop adds all combinations of abundant numbers
	for (int i = 0; i < abundant_numbers.size(); i++)
	{
		for (int j = i; j < abundant_numbers.size(); j++)
		{
			int num = abundant_numbers[i] + abundant_numbers[j];
			if (num >= maximum) break;
			divisor_sums[num] = 1; //normally would just add directly to answer, but not sure if there will be any double counting
		}
	}

	for (int i = 0; i < divisor_sums.size(); i++) //start at zero because 0 counts
		if (!divisor_sums[i]) answer += i;
	//ran in 0.0086153 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 4179871
	//ran in 0.0086153 seconds
}