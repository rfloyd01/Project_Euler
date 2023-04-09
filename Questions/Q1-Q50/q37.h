#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes p_sieve(), <vector>
#include <algorithm>

//Truncatable primes
std::pair<std::string, double> q37()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0, limit = 1000000000, ten_pow = 10;

	std::vector<int> right_primes = { 2, 3, 5, 7 }, left_primes = { 2, 3, 5, 7 };
	std::vector<int> useable_numbers = { 1, 3, 7, 9 }, non_save_numbers = { 2, 5 };

	std::vector<bool> right_prims = p_sieve(1000000);
	std::vector<bool> left_prims = right_prims;

	while (true)
	{
		if (right_primes.size() == 0 || left_primes.size() == 0) break;

		std::vector<int> new_right_primes, new_left_primes;
		for (int i = 0; i < right_primes.size(); i++)
		{
			for (int j = 0; j < useable_numbers.size(); j++)
			{
				int prime_candidate = 10 * right_primes[i] + useable_numbers[j];
				if (primeNumberTest(prime_candidate)) new_right_primes.push_back(prime_candidate);
			}
		}
		for (int i = 0; i < left_primes.size(); i++)
		{
			for (int j = 0; j < useable_numbers.size(); j++)
			{
				int prime_candidate = ten_pow * useable_numbers[j] + left_primes[i];
				if (primeNumberTest(prime_candidate)) new_left_primes.push_back(prime_candidate);
			}
			for (int j = 0; j < non_save_numbers.size(); j++)
			{
				int prime_candidate = ten_pow * non_save_numbers[j] + left_primes[i];
				if (primeNumberTest(prime_candidate))
				{
					if (std::find(new_right_primes.begin(), new_right_primes.end(), prime_candidate) != new_right_primes.end())
					{
						if (prime_candidate < limit) answer += prime_candidate;
					}
				}
			}
		}

		for (int i = 0; i < new_right_primes.size(); i++)
		{
			if (std::find(new_left_primes.begin(), new_left_primes.end(), new_right_primes[i]) != new_left_primes.end())
			{
				if (new_right_primes[i] < limit) answer += new_right_primes[i];
			}
		}

		right_primes = new_right_primes;
		left_primes = new_left_primes;
		ten_pow *= 10;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 748317
	//ran in 0.0002138 seconds
}

//NOTES
//A few observations off the bat, none of the numbers can have any even digits because at some point one of these digits will be at the very end of the number and thus be
//divisible by 2. The only acception to this is that the largest number of the chain can start with a 2. None of the answers can have a 5 in it, because anything that ends
//in a 5 will also be divisible by 5. Not sure what the largest possible number is so set an upper limit of 1,000,000. Also, as soon as a number fails either the right or
//left test, that number is set to zero in the overall prime array. Since it's known to eventually lead to a false value, no need to check it again in the future. Decided
//to make separate arrays for numbers that are left and right truncatable, as some numbers that would be eliminated by the right truncate function such as 97 wouldn't be
//eliminated from the left truncate function.

//HACKERRANK UPDATE
//Build the primes dynamically which eliminates the need for a sieve, and also lets us know when no more answer can be found as both arrays we use to build will be empty.