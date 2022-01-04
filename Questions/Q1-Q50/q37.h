#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes p_sieve(), <vector>

//Truncatable primes
bool leftTest(int p, std::vector<bool> &primes)
{
	//keep removing digits from the left side of p and if a non-prime is obtained return 0, otherwise return 1
	int length, new_p = p;
	if (p < 10) length = 1;
	else if (p < 100) length = 10;
	else if (p < 1000) length = 100;
	else if (p < 10000) length = 1000;
	else if (p < 100000) length = 10000;
	else if (p < 1000000) length = 100000;

	while (new_p > 0)
	{
		if (!primes[new_p])
		{
			primes[p] = 0; //turn the current prime being tested to 0 so future calls will know that it isn't left truncatable
			return 0;
		}
		new_p %= length;
		length /= 10;
	}
	return 1;
}

bool rightTest(int p, std::vector<bool>& primes)
{
	//keep removing digits from the right side of p and if a non-prime is obtained return 0, otherwise return 1
	int new_p = p;
	while (new_p > 0)
	{
		if (!primes[new_p])
		{
			primes[p] = 0;
			return 0;
		}
		new_p /= 10;
	}
	return 1;
}

std::pair<std::string, double> q37()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	std::vector<bool> right_prims = p_sieve(1000000);
	std::vector<bool> left_prims = right_prims;
	for (int i = 10; i < right_prims.size(); i++)
	{
		if (right_prims[i])
		{
			if (rightTest(i, right_prims)) //right test should run quicker than left test so do it first
			{
				if (leftTest(i, left_prims))
				{
					answer += i;
				}
			}
		}
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 748317
	//ran in 0.0330662 seconds
}

//NOTES
//A few observations off the bat, none of the numbers can have any even digits because at some point one of these digits will be at the very end of the number and thus be
//divisible by 2. The only acception to this is that the largest number of the chain can start with a 2. None of the answers can have a 5 in it, because anything that ends
//in a 5 will also be divisible by 5. Not sure what the largest possible number is so set an upper limit of 1,000,000. Also, as soon as a number fails either the right or
//left test, that number is set to zero in the overall prime array. Since it's known to eventually lead to a false value, no need to check it again in the future. Decided
//to make separate arrays for numbers that are left and right truncatable, as some numbers that would be eliminated by the right truncate function such as 97 wouldn't be
//eliminated from the left truncate function.