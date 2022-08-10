#pragma once

#include <Header_Files/pch.h>
#include <vector>
#include <map>

//Longest Collatz sequence
int ChainLength(long long n, int* used_numbers)
{
	if (n >= 1000000)
	{
		if (n % 2 == 0) return (1 + ChainLength(n / 2, used_numbers));
		else return (2 + ChainLength((3 * n + 1) / 2, used_numbers));
	}
	else
	{
		if (used_numbers[n]) return used_numbers[n];

		if (n % 2 == 0) used_numbers[n] = 1 + ChainLength(n / 2, used_numbers);
		else used_numbers[n] = 2 + ChainLength((3 * n + 1) / 2, used_numbers);

		return used_numbers[n];
	}
}

std::pair<std::string, double> q14()
{
	//had overflow issues so used long integers
	auto run_time = std::chrono::steady_clock::now();

	int answer = 0, longest_chain = 0;

	//create hash table for numbers already calculated below 1000000, initialize 1 to a length of 1
	int* used_numbers = new int[1000000]();
	used_numbers[1] = 1;

	for (int i = 2; i < 1000000; i++)
	{
		int current_length = ChainLength(i, used_numbers);
		if (current_length > longest_chain)
		{
			longest_chain = current_length;
			answer = i;
		}
	}

	delete[] used_numbers; //erase from heap
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 837799
	//ran in 0.0089371 seconds
}

//NOTES
//Without using any kind of caching a brute force approach can run in about .2 seconds, however, I want to try and cache already calculated numbers to avoid doing repeat
//calculations. I kept getting overflow issues at first and realized that some of the numbers in the sequence were going well over the limit. My first thought was to store
//used values in a map instead of an array to handle the larger numbers but this made the program run much much slower (clocking in at a whopping 1.7 seconds). Instead I decided 
//to only kept track of calculated numbers below the limit of 1,000,000 and this offered the speedup I was looking for. Looking in the forums showed that no numbers below 500,000
//could be the answer so the search loop can start at 500,000 instead of one. Furthermore, if n is odd then (3n + 1) will always be even and a step can be saved because an even number
//will always have a chain length one more than itself divided by 2. Therefore an odd number will always have a chain length that's 2 greater than (3*n + 1) / 2;