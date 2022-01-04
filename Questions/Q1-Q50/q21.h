#pragma once

#include <Header_Files/pch.h>
#include <vector>

//Amicable numbers
std::pair<std::string, double> q21()
{
	//use a seive to quickly figure out the sum of all divisors for number 2 - maximum, then just look for matches
	//not positive if this actually happens, but some amicable numbers may fall outside of the maximum range (i.e. d(a) = b and d(b) = a, but b > 10,000 and a is < 10,000)
	//use a larger vector than necessary to make sure the above possible case is accounted for

	auto run_time = std::chrono::steady_clock::now();

	int answer = 0, limit = 10000;
	int maximum = limit * 10; //wasn't sure how large to make the maximum so just made it 10x larger than the largest number (10,000)
	std::vector<int> divisor_sums;
	for (int i = 0; i < maximum; i++) divisor_sums.push_back(1); //use a 1 because every number is divisible by 1
	divisor_sums[0] = 0; divisor_sums[1] = 0;

	//use a sieve to calculate divisor sums for each number under the limit
	//we can stop the search when 2 * i > limit
	for (int i = 2; i < (limit / 2); i++)
		for (int j = i + i; j < maximum; j += i) divisor_sums[j] += i;

	//check for amicable numbers in the sieve
	for (int a = 0; a < limit; a++)
	{
		int b = divisor_sums[a];
		if (divisor_sums[b] == a)
			if (a != b) answer += a; //make sure to exclude numbers that are amicable with themselves
	}
	//ran in 0.0008313 seconds

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 31626
	//ran in 0.0008313 seconds
}
