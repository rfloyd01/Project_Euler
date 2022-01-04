#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes primes()

//Prime Summations
std::pair<std::string, double> q77()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	std::vector<int> prims = primes(100);

	int partitions[101] = { 0 };
	partitions[0] = 1;

	for (int i = 0; i < prims.size(); i++)
		for (int j = prims[i]; j < 101; j++) partitions[j] += partitions[j - prims[i]];
	
	for (int i = 0; i < 101; i++)
	{
		if (partitions[i] >= 5000)
		{
			answer = i;
			break;
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 71
	//ran in 0.0000179 seconds
}

//NOTES
//This problem is basically the same as 76 (which was in turn the same as 31) although instead of adding all of the numbers together we only care about prime numbers.
//The main difference between this problem and 76 is that we don't know how high up we need to go. Looking at the problem text, there are 5 instances of partitions of
//10 that feature only primes. Plugging 10 into question 76 shows that there are 41 total partitions, so roughly an eigth of the total were primes only. I think this
//ratio will decrease as n gets larger (for example, 5 has 2/7 of its total partitions as primes only) so using a ratio of 1/8 should be a safe bet, i.e. we should really
//only need to search numbers that have 8 * 5,000 = 40,000 partitions. Considering that 100 has almost 200,000,000 partitions, the answer to this problem will almost
//definitely be lower than 100, but we know that the run time for 100 was basically instant so just keep that as the ceiling for this problem. It turns out the answer
//is 71 which is indeed lower than 100, but is actually much closer than I expected it to be. 71 has ~4,700,000 distinct partitions so that fact that only ~5,000 of them
//featured distinctly primes means that closer to 1 in 1,000 partitions was prime only which is a far cry from the 1 in 8 ratio for the partitions of 10. My estimate 
//ended up being way off so it's a good thing I went up to 100.